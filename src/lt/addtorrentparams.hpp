#ifndef _HDKN_LT_ADDTORRENTPARAMS_HPP
#define _HDKN_LT_ADDTORRENTPARAMS_HPP

#include <node.h>
#include <node_object_wrap.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/session.hpp>

namespace lt
{
    class TorrentInfo;

    class AddTorrentParams : public node::ObjectWrap
    {
    public:
        static void Initialize(v8::Handle<v8::Object> exports);
        
        libtorrent::add_torrent_params* params_;

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void GetTorrentInfo(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetTorrentInfo(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetName(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetName(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetSavePath(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetSavePath(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetTrackerId(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetTrackerId(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetUrl(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetUrl(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetUuid(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetUuid(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetSourceFeedUrl(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetSourceFeedUrl(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void GetMaxUploads(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetMaxUploads(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetMaxConnections(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetMaxConnections(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetUploadLimit(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetUploadLimit(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void GetDownloadLimit(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetDownloadLimit(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    private:
        explicit AddTorrentParams(libtorrent::add_torrent_params* session);
        ~AddTorrentParams();

        TorrentInfo* ti_;
    };
}

#endif
