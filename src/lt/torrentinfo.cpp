#include "torrentinfo.hpp"

#include <env.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <libtorrent/torrent_info.hpp>

#define HDKN_SET_PROTO_PROPGET(tpl, name, func) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), func)

using namespace lt;
using namespace v8;

v8::Persistent<v8::Function> TorrentInfo::ctor_;

TorrentInfo::TorrentInfo(libtorrent::torrent_info* info)
    : info_(info)
{
}

TorrentInfo::~TorrentInfo()
{
    if (info_ != nullptr)
    {
        delete info_;
    }
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
    HDKN_SET_PROTO_PROPGET(t, "numPieces", GetNumPieces);
    HDKN_SET_PROTO_PROPGET(t, "totalSize", GetTotalSize);
    HDKN_SET_PROTO_PROPGET(t, "pieceLength", GetPieceLength);
    HDKN_SET_PROTO_PROPGET(t, "infoHash", GetInfoHash);
    HDKN_SET_PROTO_PROPGET(t, "numFiles", GetNumFiles);
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
    HDKN_SET_PROTO_PROPGET(t, "comment", GetComment);
    HDKN_SET_PROTO_PROPGET(t, "creator", GetCreator);
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

void TorrentInfo::NewInstance(const PropertyCallbackInfo<Value>& info, libtorrent::torrent_info* ti)
{
    Isolate* isolate = info.GetIsolate();
    Local<Value> argv[1] = { External::New(isolate, ti) };
    Local<Function> ctor = Local<Function>::New(isolate, ctor_);
    info.GetReturnValue().Set(ctor->NewInstance(1, argv));
}

void TorrentInfo::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    libtorrent::torrent_info* info;

    if (args[0]->IsExternal())
    {
        void* ptr = args[0].As<External>()->Value();
        info = static_cast<libtorrent::torrent_info*>(ptr);
    }
    else if (node::Buffer::HasInstance(args[0]))
    {
        libtorrent::error_code ec;
        info = new libtorrent::torrent_info(node::Buffer::Data(args[0]), node::Buffer::Length(args[0]), ec);

        if (ec)
        {
            // throw error
        }
    }
    else
    {
        // throw
    }

    TorrentInfo* ti = new TorrentInfo(info);
    ti->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void TorrentInfo::GetNumPieces(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(Integer::New(info.GetIsolate(), ti->info_->num_pieces()));
}

void TorrentInfo::GetTotalSize(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), ti->info_->total_size()));
}

void TorrentInfo::GetPieceLength(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(Integer::New(info.GetIsolate(), ti->info_->piece_length()));
}

void TorrentInfo::GetInfoHash(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    std::string hash = libtorrent::to_hex(ti->info_->info_hash().to_string());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), hash.c_str()));
}

void TorrentInfo::GetNumFiles(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(Integer::New(info.GetIsolate(), ti->info_->num_files()));
}

void TorrentInfo::GetName(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), ti->info_->name().c_str()));
}

void TorrentInfo::GetComment(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), ti->info_->comment().c_str()));
}

void TorrentInfo::GetCreator(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    TorrentInfo* ti = ObjectWrap::Unwrap<TorrentInfo>(info.This());
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), ti->info_->creator().c_str()));
}
