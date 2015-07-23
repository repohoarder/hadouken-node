#include "fingerprint.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/fingerprint.hpp>
#include <libtorrent/version.hpp>

using namespace lt;
using namespace v8;

void Fingerprint::Initialize(node::Environment* env, Handle<Object> exports)
{
    Local<FunctionTemplate> t = env->NewFunctionTemplate(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    env->SetProtoMethod(t, "name", GetName);

    exports->Set(FIXED_ONE_BYTE_STRING(env->isolate(), "Fingerprint"), t->GetFunction());
}

void Fingerprint::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());

    node::Environment* env = node::Environment::GetCurrent(args);
    Fingerprint* fp = new Fingerprint(env, args.This());
    bool initialized = false;

    if (args.Length() == 5) // Fingerprint
    {
        String::Utf8Value id(args[0]->ToString());

        initialized = fp->Init(*id,
            args[1]->Int32Value(),
            args[2]->Int32Value(),
            args[3]->Int32Value(),
            args[4]->Int32Value());
    }

    if (!initialized)
    {
        return env->ThrowError("Initialization failed.");
    }
}

void Fingerprint::GetName(const FunctionCallbackInfo<Value>& args)
{
    node::Environment* env = node::Environment::GetCurrent(args);
    Fingerprint* fp = node::Unwrap<Fingerprint>(args.Holder());

    if (fp == nullptr)
    {
        args.GetReturnValue().Set(FIXED_ONE_BYTE_STRING(env->isolate(), "fp==nullptr"));
    }
    else if (fp->fp_ == nullptr)
    {
        args.GetReturnValue().Set(FIXED_ONE_BYTE_STRING(env->isolate(), "fp->fp_==nullptr"));
    }
    else
    {
        args.GetReturnValue().Set(FIXED_ONE_BYTE_STRING(env->isolate(), fp->fp_->name));
    }
}

bool Fingerprint::Init(const char* id, int major, int minor, int revision, int tag)
{
    fp_ = new libtorrent::fingerprint(id, major, minor, revision, tag);
    initialized_ = true;
    return true;
}
