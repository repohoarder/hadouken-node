#include <env.h>
#include <node.h>
#include <v8.h>

#include <libtorrent/version.hpp>

#include "lt/addtorrentparams.hpp"
#include "lt/fingerprint.hpp"
#include "lt/session.hpp"
#include "lt/torrentinfo.hpp"

using namespace v8;

namespace
{
    void Initialize(v8::Local<v8::Object> exports,
        v8::Local<v8::Value> unused,
        v8::Local<v8::Context> context,
        void* priv)
    {
        node::Environment* env = node::Environment::GetCurrent(context);
        exports->Set(String::NewFromUtf8(env->isolate(), "VERSION"), String::NewFromUtf8(env->isolate(), LIBTORRENT_VERSION));

        // Initialize stuffs
        lt::AddTorrentParams::Initialize(exports);
        lt::Fingerprint::Initialize(env, exports);
        lt::Session::Initialize(env, exports);
        lt::TorrentInfo::Initialize(exports);
    }
}

NODE_MODULE_CONTEXT_AWARE_BUILTIN(libtorrent, Initialize);
