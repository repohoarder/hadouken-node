#include <env.h>
#include <node.h>
#include <uv.h>
#include <v8-platform.h>

namespace v8 {
    namespace platform {

        /**
        * Returns a new instance of the default v8::Platform implementation.
        *
        * The caller will take ownership of the returned pointer. |thread_pool_size|
        * is the number of worker threads to allocate for background jobs. If a value
        * of zero is passed, a suitable default based on the current number of
        * processors online will be chosen.
        */
        v8::Platform* CreateDefaultPlatform(int thread_pool_size = 0);


        /**
        * Pumps the message loop for the given isolate.
        *
        * The caller has to make sure that this is called from the right thread.
        * Returns true if a task was executed, and false otherwise. This call does
        * not block if no task is pending. The |platform| has to be created using
        * |CreateDefaultPlatform|.
        */
        bool PumpMessageLoop(v8::Platform* platform, v8::Isolate* isolate);


    }  // namespace platform
}  // namespace v8

// Force all builtin modules to be referenced so they can actually run their
// DSO constructors, see http://git.io/DRIqCg.
#if defined(WIN32)
#define REFERENCE_MODULE(name) \
  __pragma(comment (linker, "/export:_register_" #name))
 #else
 #define REFERENCE_MODULE(name) \
  extern "C" void _register_ ## name (void); \
  void (*fp_register_ ## name)(void) = _register_ ## name
 #endif
// Node's builtin modules.
REFERENCE_MODULE(cares_wrap);
REFERENCE_MODULE(fs_event_wrap);
REFERENCE_MODULE(buffer);
REFERENCE_MODULE(contextify);
REFERENCE_MODULE(crypto);
REFERENCE_MODULE(fs);
REFERENCE_MODULE(http_parser);
REFERENCE_MODULE(os);
REFERENCE_MODULE(v8);
REFERENCE_MODULE(zlib);
REFERENCE_MODULE(pipe_wrap);
REFERENCE_MODULE(process_wrap);
REFERENCE_MODULE(signal_wrap);
REFERENCE_MODULE(smalloc);
REFERENCE_MODULE(spawn_sync);
REFERENCE_MODULE(tcp_wrap);
REFERENCE_MODULE(timer_wrap);
REFERENCE_MODULE(tls_wrap);
REFERENCE_MODULE(tty_wrap);
REFERENCE_MODULE(udp_wrap);
REFERENCE_MODULE(uv);
// Hadouken modules
REFERENCE_MODULE(libtorrent);

int main(int argc, char *argv[])
{
    argv = uv_setup_args(argc, argv);
    int exec_argc;
    const char** exec_argv;
    node::Init(&argc, const_cast<const char**>(argv), &exec_argc, &exec_argv);

    v8::Platform* platform = v8::platform::CreateDefaultPlatform(4);
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();

    v8::Isolate* isolate = v8::Isolate::New();
    node::node_isolate = isolate;

    int exit_code = 1;

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        node::Environment* env = node::CreateEnvironment(isolate,
            uv_default_loop(),
            context,
            argc,
            argv,
            exec_argc,
            exec_argv);

        v8::Context::Scope context_scope(context);

        node::LoadEnvironment(env);

        {
            v8::SealHandleScope seal(isolate);
            bool more;

            do
            {
                v8::platform::PumpMessageLoop(platform, isolate);
                more = uv_run(env->event_loop(), UV_RUN_ONCE);

                if (more == false)
                {
                    v8::platform::PumpMessageLoop(platform, isolate);
                    node::EmitBeforeExit(env);

                    more = uv_loop_alive(env->event_loop());

                    if (uv_run(env->event_loop(), UV_RUN_NOWAIT) != 0)
                    {
                        more = true;
                    }
                }
            } while (more == true);
        }

        exit_code = node::EmitExit(env);
        node::RunAtExit(env);

        env->Dispose();
    }

    v8::V8::Dispose();

    delete platform;
    delete[] exec_argv;

    return exit_code;
}
