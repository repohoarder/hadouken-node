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
        explicit TorrentInfo(boost::intrusive_ptr<libtorrent::torrent_info> info);

        static void Initialize(v8::Handle<v8::Object> exports);
        
        boost::intrusive_ptr<libtorrent::torrent_info> info_;

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void GetName(v8::Local<v8::String> prop, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        ~TorrentInfo();
    };
}

#endif
