#include "torrentinfo.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/torrent_info.hpp>

#define HDKN_SET_PROTO_PROPGET(tpl, name, func) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), func)

using namespace lt;
using namespace v8;

v8::Persistent<v8::Function> TorrentInfo::ctor_;

TorrentInfo::TorrentInfo(boost::intrusive_ptr<libtorrent::torrent_info> info)
    : info_(info)
{
}

TorrentInfo::~TorrentInfo()
{
}

void TorrentInfo::Initialize(Handle<Object> exports)
{
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, New);
    t->SetClassName(String::NewFromUtf8(isolate, "TorrentInfo"));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    // files
    // orig_files
    // rename_file
    // remap_file
    // trackers
    // add_tracker
    // add_url_seed
    // web_seeds
    // add_http_seed
    // num_pieces
    // total_size
    // piece_length
    // info_hash
    // num_files
    // file_at
    // map_block
    // map_file
    // ssl_cert
    // is_valid
    // priv
    // isI2p
    // hash_for_piece
    // piece_size
    // merkle_tree
    // set_merkle_tree
    // creation_date
    HDKN_SET_PROTO_PROPGET(t, "name", GetName);
    // comment
    // creator
    // nodes
    // add_node
    // parse_info_section
    // info
    // swap
    // metadata_size
    // metadata
    // is_merkle_torrent

    ctor_.Reset(isolate, t->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "TorrentInfo"), t->GetFunction());
}

void TorrentInfo::NewInstance(const PropertyCallbackInfo<Value>& info, boost::intrusive_ptr<libtorrent::torrent_info> ti)
{
}

void TorrentInfo::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    String::Utf8Value val(args[0]->ToString());

    libtorrent::error_code ec;
    TorrentInfo* params = new TorrentInfo(new libtorrent::torrent_info(*val, ec));

    if (ec)
    {
        // throw error
    }

    params->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void TorrentInfo::GetName(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), ti->info_->name().c_str()));
}
