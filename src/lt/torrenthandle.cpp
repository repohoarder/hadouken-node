#include "torrenthandle.hpp"

#include <env.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <libtorrent/torrent_handle.hpp>

#include "torrentstatus.hpp"

#define HDKN_SET_PROTO_PROPGET(tpl, name, func) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), func)

using namespace lt;
using namespace v8;

v8::Persistent<v8::Function> TorrentHandle::ctor_;

TorrentHandle::TorrentHandle(libtorrent::torrent_handle* handle)
    : handle_(handle)
{
}

TorrentHandle::~TorrentHandle()
{
    if (handle_ != nullptr)
    {
        delete handle_;
    }
}

void TorrentHandle::Initialize(Handle<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, New);
    t->SetClassName(String::NewFromUtf8(isolate, "TorrentHandle"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    // add_piece
    // read_piece
    // have_piece
    // get_peer_info
    NODE_SET_PROTOTYPE_METHOD(t, "getStatus", GetStatus);
    // get_download_queue
    // reset_piece_deadline
    // clear_piece_deadlines
    // set_piece_deadline
    // set_priority
    // file_progress
    // clear_error
    // trackers
    // replace_trackers
    // add_tracker
    // add_url_seed
    // remove_url_seed
    // url_seeds
    // add_http_seed
    // remove_http_seed
    // http_seeds
    // add_extension
    // set_metadata
    // is_valid
    // pause
    // resume
    // set_upload_mode
    // set_share_mode
    // flush_cache
    // apply_ip_filter
    // force_recheck
    // save_resume_data
    // need_save_resume_data
    // auto_managed
    // queue_position_down
    // queue_position_top
    // queue_position
    // queue_position_bottom
    // queue_position_up
    // resolve_countries
    // set_ssl_certificate
    // get_storage_impl
    // torrent_file
    // use_interface
    // piece_availability
    // piece_priority
    // prioritize_pieces
    // piece_priorities
    // file_priority
    // prioritize_files
    // file_priority(set)
    // file_priorities
    // force_reannounce
    // force_dht_announce
    // scrape_tracker
    // upload_limit
    // download_limit
    // set_upload_limit
    // set_download_limit
    // set_sequential_download
    // connect_peer
    // max_uploads
    // set_max_uploads
    // max_connections
    // set_max_connections
    // set_tracker_login
    // move_storage
    // rename_file
    // super_seeding
    HDKN_SET_PROTO_PROPGET(t, "infoHash", GetInfoHash);

    ctor_.Reset(isolate, t->GetFunction());
}

void TorrentHandle::NewInstance(const FunctionCallbackInfo<Value>& info, libtorrent::torrent_handle* th)
{
    Isolate* isolate = info.GetIsolate();
    Local<Value> argv[1] = { External::New(isolate, th) };
    Local<Function> ctor = Local<Function>::New(isolate, ctor_);
    info.GetReturnValue().Set(ctor->NewInstance(1, argv));
}

void TorrentHandle::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    libtorrent::torrent_handle* th;

    if (args[0]->IsExternal())
    {
        void* ptr = args[0].As<External>()->Value();
        th = static_cast<libtorrent::torrent_handle*>(ptr);
    }
    else
    {
        // throw
    }

    TorrentHandle* handle = new TorrentHandle(th);
    handle->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void TorrentHandle::GetStatus(const FunctionCallbackInfo<Value>& info)
{
    TorrentHandle* th = ObjectWrap::Unwrap<TorrentHandle>(info.This());
    libtorrent::torrent_status status = th->handle_->status();
    TorrentStatus::NewInstance(info, new libtorrent::torrent_status(status));
}

void TorrentHandle::GetInfoHash(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentHandle* th = ObjectWrap::Unwrap<TorrentHandle>(info.This());
    std::string hash = libtorrent::to_hex(th->handle_->info_hash().to_string());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), hash.c_str()));
}
