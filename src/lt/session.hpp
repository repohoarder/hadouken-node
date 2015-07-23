#ifndef _HDKN_LT_SESSION_HPP
#define _HDKN_LT_SESSION_HPP

#include <node.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/session.hpp>

namespace lt
{
    class Fingerprint;

    class Session : public node::BaseObject
    {
    public:
        Session(node::Environment* env, v8::Local<v8::Object> wrap)
            : BaseObject(env, wrap),
            initialized_(false),
            session_(0)
        {
        }

        ~Session() override
        {
            if (session_ != nullptr)
            {
                delete session_;
            }
        }

        static void Initialize(node::Environment* env, v8::Handle<v8::Object> exports);

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void IsPaused(const v8::FunctionCallbackInfo<v8::Value>& args);

        bool Init();

        bool Init(Fingerprint* fingerprint);

    private:
        bool initialized_;
        libtorrent::session* session_;
    };
}

#endif
