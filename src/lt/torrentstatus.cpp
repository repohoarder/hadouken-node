#include "torrentstatus.hpp"

#include <env.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <libtorrent/torrent_handle.hpp>

#define HDKN_SET_PROTO_PROPGET(tpl, name, func) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), func)

using namespace lt;
using namespace v8;

v8::Persistent<v8::Function> TorrentStatus::ctor_;

TorrentStatus::TorrentStatus(libtorrent::torrent_status* status)
    : status_(status)
{
}

TorrentStatus::~TorrentStatus()
{
    if (status_ != nullptr)
    {
        delete status_;
    }
}

void TorrentStatus::Initialize(Handle<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, New);
    t->SetClassName(String::NewFromUtf8(isolate, "TorrentStatus"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    // handle
    // error
    // save_path
    HDKN_SET_PROTO_PROPGET(t, "name", GetName);
    // torrent_file
    // next_announce
    // announce_interval
    // current_tracker
    // total_download
    // total_upload
    // total_payload_download
    // total_payload_upload
    // total_failed_bytes
    // total_reduntant_bytes
    // pieces
    // verified_pieces
    // total_done
    // total_wanted_done
    // total_wanted
    // all_time_upload
    // all_time_download
    // added_time
    // completed_time
    // last_seen_completed
    // storage_mode
    HDKN_SET_PROTO_PROPGET(t, "progress", GetProgress);
    // progress_ppm
    // queue_position
    // download_rate
    // upload_rate
    // download_payload_rate
    // upload_payload_rate
    // num_seeds
    // num_peers
    // num_complete
    // num_incomplete
    // list_seeds
    // list_peers
    // connect_candidates
    // num_pieces
    // distributed_full_copies
    // distributed_fraction
    // distributed_copies
    // block_size
    // num_uploads
    // num_connections
    // uploads_limit
    // connections_limit
    // up_bandwidth_queue
    // down_bandwidth_queue
    // time_since_upload
    // time_since_download
    // active_time
    // finished_time
    // seeding_time
    // seed_rank
    // last_scrape
    // sparse_regions
    // priority
    // state
    // need_save_resume
    // ip_filter_applies
    // upload_mode
    // share_mode
    // super_seeding
    // paused
    // auto_managed
    // sequential_download
    // is_seeding
    // is_finished
    // has_metadata
    // has_incoming
    // seed_mode
    // moving_storage
    HDKN_SET_PROTO_PROPGET(t, "infoHash", GetInfoHash);

    ctor_.Reset(isolate, t->GetFunction());
}

void TorrentStatus::NewInstance(const FunctionCallbackInfo<Value>& info, libtorrent::torrent_status* status)
{
    Isolate* isolate = info.GetIsolate();
    Local<Value> argv[1] = { External::New(isolate, status) };
    Local<Function> ctor = Local<Function>::New(isolate, ctor_);
    info.GetReturnValue().Set(ctor->NewInstance(1, argv));
}

void TorrentStatus::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    libtorrent::torrent_status* status;

    if (args[0]->IsExternal())
    {
        void* ptr = args[0].As<External>()->Value();
        status = static_cast<libtorrent::torrent_status*>(ptr);
    }
    else
    {
        // throw
    }

    TorrentStatus* ts = new TorrentStatus(status);
    ts->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void TorrentStatus::GetName(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentStatus* status = ObjectWrap::Unwrap<TorrentStatus>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), status->status_->name.c_str()));
}

void TorrentStatus::GetProgress(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentStatus* status = ObjectWrap::Unwrap<TorrentStatus>(info.This());
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), status->status_->progress));
}

void TorrentStatus::GetInfoHash(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentStatus* status = ObjectWrap::Unwrap<TorrentStatus>(info.This());
    std::string hash = libtorrent::to_hex(status->status_->info_hash.to_string());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), hash.c_str()));
}
