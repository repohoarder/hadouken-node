#ifndef _HDKN_LT_SESSION_HPP
#define _HDKN_LT_SESSION_HPP

#include <node.h>
#include <node_object_wrap.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/session.hpp>

namespace lt
{
    class Fingerprint;

    class Session : public node::ObjectWrap
    {
    public:
        static void Initialize(node::Environment* env, v8::Handle<v8::Object> exports);

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void IsPaused(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        explicit Session(libtorrent::session* session = 0);
        ~Session();

        libtorrent::session* session_;
    };
}

#endif
