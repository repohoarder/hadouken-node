#include "session.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/session.hpp>

#include "fingerprint.hpp"

using namespace lt;
using namespace v8;

void Session::Initialize(node::Environment* env, Handle<Object> exports)
{
    Local<FunctionTemplate> t = env->NewFunctionTemplate(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    env->SetProtoMethod(t, "isPaused", IsPaused);

    exports->Set(FIXED_ONE_BYTE_STRING(env->isolate(), "Session"), t->GetFunction());
}

void Session::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    node::Environment* env = node::Environment::GetCurrent(args);
    Session* sess = new Session(env, args.This());
    bool initialized = false;

    if (args.Length() == 1) // Fingerprint
    {
        Fingerprint* fp = node::Unwrap<Fingerprint>(args[0].As<v8::Object>());
        initialized = sess->Init(fp);
    }
    else
    {
        initialized = sess->Init();
    }

    if (!initialized)
    {
        return env->ThrowError("Initialization failed.");
    }
}

void Session::IsPaused(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    Session* sess = node::Unwrap<Session>(args.Holder());
}

bool Session::Init()
{
    session_ = new libtorrent::session();
    initialized_ = true;
    return true;
}

bool Session::Init(Fingerprint* fingerprint)
{
    session_ = new libtorrent::session(*fingerprint->fp_);
    initialized_ = true;
    return true;
}
