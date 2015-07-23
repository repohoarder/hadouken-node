#include "session.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/session.hpp>

#include "fingerprint.hpp"

using namespace lt;
using namespace v8;

Session::Session(libtorrent::session* session)
    : session_(session)
{
}

Session::~Session()
{
    if (session_ != nullptr)
    {
        delete session_;
    }
}

void Session::Initialize(node::Environment* env, Handle<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, New);
    t->SetClassName(String::NewFromUtf8(isolate, "Session"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    t->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, "isPaused"), IsPaused);

    exports->Set(String::NewFromUtf8(isolate, "Session"), t->GetFunction());
}

void Session::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    Session* sess = new Session(new libtorrent::session());
    sess->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void Session::IsPaused(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(info.GetIsolate());
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), session->session_->is_paused()));
}

