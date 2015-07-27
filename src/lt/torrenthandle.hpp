#ifndef _HDKN_LT_TORRENTHANDLE_HPP
#define _HDKN_LT_TORRENTHANDLE_HPP

#include <node.h>
#include <node_object_wrap.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/torrent_handle.hpp>

namespace lt
{
    class TorrentHandle : public node::ObjectWrap
    {
    public:
        static void Initialize(v8::Handle<v8::Object> exports);

        static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info, libtorrent::torrent_handle* ti);

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void GetStatus(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void GetInfoHash(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        explicit TorrentHandle(libtorrent::torrent_handle* info);
        ~TorrentHandle();

        static v8::Persistent<v8::Function> ctor_;
        libtorrent::torrent_handle* handle_;
    };
}

#endif
