#ifndef _HDKN_LT_TORRENTINFO_HPP
#define _HDKN_LT_TORRENTINFO_HPP

#include <node.h>
#include <node_object_wrap.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/torrent_info.hpp>

namespace lt
{
    class TorrentInfo : public node::ObjectWrap
    {
    public:
        static void Initialize(v8::Handle<v8::Object> exports);

        static void NewInstance(const v8::PropertyCallbackInfo<v8::Value>& info, libtorrent::torrent_info* ti);
        
        libtorrent::torrent_info* info_;

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void GetNumPieces(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetTotalSize(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetPieceLength(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetInfoHash(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetNumFiles(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetName(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetComment(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void GetCreator(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        explicit TorrentInfo(libtorrent::torrent_info* info);
        ~TorrentInfo();

        static v8::Persistent<v8::Function> ctor_;
    };
}

#endif
