#include "addtorrentparams.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/session.hpp>

#include "torrentinfo.hpp"

#define HDKN_SET_PROTO_PROP(tpl, name, get, set) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), get, set)

using namespace lt;
using namespace v8;

AddTorrentParams::AddTorrentParams(libtorrent::add_torrent_params* params)
    : params_(params)
{
}

AddTorrentParams::~AddTorrentParams()
{
    if (params_ != nullptr)
    {
        delete params_;
    }
}

void AddTorrentParams::Initialize(Handle<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, New);
    t->SetClassName(String::NewFromUtf8(isolate, "AddTorrentParams"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    // version
    HDKN_SET_PROTO_PROP(t, "ti", GetTorrentInfo, SetTorrentInfo);
    // trackers
    // url_seeds
    // dht_nodes
    HDKN_SET_PROTO_PROP(t, "name", GetName, SetName);
    HDKN_SET_PROTO_PROP(t, "savePath", GetSavePath, SetSavePath);
    // resume_data
    // storage_mode
    // storage
    // userdata
    // file_priorities
    HDKN_SET_PROTO_PROP(t, "trackerId", GetTrackerId, SetTrackerId);
    HDKN_SET_PROTO_PROP(t, "url", GetUrl, SetUrl);
    HDKN_SET_PROTO_PROP(t, "uuid", GetUuid, SetUuid);
    HDKN_SET_PROTO_PROP(t, "sourceFeedUrl", GetSourceFeedUrl, SetSourceFeedUrl);
    // flags
    // info_hash
    HDKN_SET_PROTO_PROP(t, "maxUploads", GetMaxUploads, SetMaxUploads);
    HDKN_SET_PROTO_PROP(t, "maxConnections", GetMaxConnections, SetMaxConnections);
    HDKN_SET_PROTO_PROP(t, "uploadLimit", GetUploadLimit, SetUploadLimit);
    HDKN_SET_PROTO_PROP(t, "downloadLimit", GetDownloadLimit, SetDownloadLimit);

    exports->Set(String::NewFromUtf8(isolate, "AddTorrentParams"), t->GetFunction());
}

void AddTorrentParams::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    AddTorrentParams* params = new AddTorrentParams(new libtorrent::add_torrent_params());
    params->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void AddTorrentParams::GetTorrentInfo(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(External::New(info.GetIsolate(), p->ti_));
}

void AddTorrentParams::SetTorrentInfo(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(value->ToObject());
    p->params_->ti = ti->info_;
}

void AddTorrentParams::GetName(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->name.c_str()));
}

void AddTorrentParams::SetName(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->name = *val;
}

void AddTorrentParams::GetSavePath(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->save_path.c_str()));
}

void AddTorrentParams::SetSavePath(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->save_path = *val;
}

void AddTorrentParams::GetTrackerId(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->trackerid.c_str()));
}

void AddTorrentParams::SetTrackerId(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->trackerid = *val;
}

void AddTorrentParams::GetUrl(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->url.c_str()));
}

void AddTorrentParams::SetUrl(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->url = *val;
}

void AddTorrentParams::GetUuid(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->uuid.c_str()));
}

void AddTorrentParams::SetUuid(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->uuid = *val;
}

void AddTorrentParams::GetSourceFeedUrl(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), p->params_->source_feed_url.c_str()));
}

void AddTorrentParams::SetSourceFeedUrl(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    String::Utf8Value val(value->ToString());
    p->params_->source_feed_url = *val;
}

void AddTorrentParams::GetMaxUploads(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(p->params_->max_uploads);
}

void AddTorrentParams::SetMaxUploads(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    p->params_->max_uploads = value->ToInt32()->Int32Value();
}

void AddTorrentParams::GetMaxConnections(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(p->params_->max_connections);
}

void AddTorrentParams::SetMaxConnections(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    p->params_->max_connections = value->ToInt32()->Int32Value();
}

void AddTorrentParams::GetUploadLimit(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(p->params_->upload_limit);
}

void AddTorrentParams::SetUploadLimit(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    p->params_->upload_limit = value->ToInt32()->Int32Value();
}

void AddTorrentParams::GetDownloadLimit(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    info.GetReturnValue().Set(p->params_->download_limit);
}

void AddTorrentParams::SetDownloadLimit(Local<String> prop, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
    AddTorrentParams* p = ObjectWrap::Unwrap<AddTorrentParams>(info.This());
    p->params_->download_limit = value->ToInt32()->Int32Value();
}
