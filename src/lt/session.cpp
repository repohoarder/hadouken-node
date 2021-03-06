#include "session.hpp"

#include <env.h>
#include <node.h>
#include <v8.h>
#include <libtorrent/session.hpp>

#include "addtorrentparams.hpp"
#include "fingerprint.hpp"
#include "torrenthandle.hpp"

#define HDKN_SET_PROTO_PROPGET(tpl, name, func) \
    tpl->PrototypeTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), func)

using namespace lt;
using namespace v8;

uv_async_t alert_async;

struct dispatch_data {
    Session* session;
    libtorrent::alert* alert;
};

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

    // load_state
    // save_state
    // refresh_torrent_status
    // get_torrent_status
    // post_torrent_updates
    // find_torrent
    NODE_SET_PROTOTYPE_METHOD(t, "getTorrents", GetTorrents);
    // async_add_torrent
    NODE_SET_PROTOTYPE_METHOD(t, "addTorrent", AddTorrent);
    // abort
    NODE_SET_PROTOTYPE_METHOD(t, "resume", Resume);
    NODE_SET_PROTOTYPE_METHOD(t, "pause", Pause);
    HDKN_SET_PROTO_PROPGET(t, "isPaused", IsPaused);
    // status
    // get_cache_status
    // get_cache_info
    // add_feed
    // remove_feed
    // get_feeds
    HDKN_SET_PROTO_PROPGET(t, "isDhtRunning", IsDhtRunning);
    // start_dht
    // stop_dht
    // set_dht_settings
    // add_dht_router
    // add_dht_node
    // dht_get_item
    // dht_put_Item
    // add_extension
    // load_country_db
    // load_asnum_db
    // as_for_ip
    // get_ip_filter
    // set_ip_filter
    // set_port_filter
    // id
    // set_peer_id
    // set_key
    // listen_on
    HDKN_SET_PROTO_PROPGET(t, "isListening", IsListening);
    HDKN_SET_PROTO_PROPGET(t, "listenPort", ListenPort);
    HDKN_SET_PROTO_PROPGET(t, "sslListenPort", SslListenPort);
    // remove_torrent
    // settings
    // set_settings
    // get_pe_settings
    // set_pe_settings
    // set_proxy
    // proxy
    // i2p_proxy
    // set_i2p_proxy
    // pop_alerts
    // pop_alert
    // wait_for_item
    // set_alert_mask
    // set_alert_dispatch (EventEmitter this)
    NODE_SET_PROTOTYPE_METHOD(t, "stopLsd", StopLsd);
    NODE_SET_PROTOTYPE_METHOD(t, "startLsd", StartLsd);
    NODE_SET_PROTOTYPE_METHOD(t, "stopUpnp", StopUpnp);
    NODE_SET_PROTOTYPE_METHOD(t, "startUpnp", StartUpnp);
    // start_upnp
    // delete_port_mapping
    // add_port_mapping
    NODE_SET_PROTOTYPE_METHOD(t, "stopNatPmp", StopNatPmp);
    NODE_SET_PROTOTYPE_METHOD(t, "startNatPmp", StartNatPmp);

    // alert dispatch async
    uv_async_init(uv_default_loop(), &alert_async, HandleAlert);

    exports->Set(String::NewFromUtf8(isolate, "Session"), t->GetFunction());
}

void Session::New(const FunctionCallbackInfo<Value>& args)
{
    CHECK(args.IsConstructCall());
    Isolate* isolate = args.GetIsolate();

    Session* sess = new Session(new libtorrent::session());

    sess->Wrap(args.This());
    sess->session_->set_alert_mask(libtorrent::alert::category_t::all_categories);
    sess->session_->set_alert_dispatch(std::bind(AlertDispatch, sess, std::placeholders::_1));

    args.GetReturnValue().Set(args.This());
}

void Session::GetTorrents(const FunctionCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    std::vector<libtorrent::torrent_handle> torrents = session->session_->get_torrents();
}

void Session::AddTorrent(const FunctionCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    AddTorrentParams* params = ObjectWrap::Unwrap<AddTorrentParams>(info[0]->ToObject());

    libtorrent::torrent_handle handle = session->session_->add_torrent(*params->params_);
    TorrentHandle::NewInstance(info, new libtorrent::torrent_handle(handle));
}

void Session::Resume(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->resume();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::Pause(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->pause();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::IsPaused(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), session->session_->is_paused()));
}

void Session::IsDhtRunning(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), session->session_->is_dht_running()));
}

void Session::IsListening(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), session->session_->is_listening()));
}

void Session::ListenPort(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(session->session_->listen_port());
}

void Session::SslListenPort(Local<String> prop, const PropertyCallbackInfo<Value>& info)
{
    Session* session = ObjectWrap::Unwrap<Session>(info.This());
    info.GetReturnValue().Set(session->session_->ssl_listen_port());
}

void Session::StopLsd(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->stop_lsd();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::StartLsd(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->start_lsd();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::StopUpnp(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->stop_upnp();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::StartUpnp(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->start_upnp();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::StopNatPmp(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->stop_natpmp();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::StartNatPmp(const FunctionCallbackInfo<Value>& info)
{
    ObjectWrap::Unwrap<Session>(info.This())->session_->start_natpmp();
    info.GetReturnValue().Set(Undefined(info.GetIsolate()));
}

void Session::AlertDispatch(Session* session, std::auto_ptr<libtorrent::alert> alert)
{
    dispatch_data* data = new dispatch_data();
    data->alert = alert.release();
    data->session = session;

    alert_async.data = data;
    uv_async_send(&alert_async);
}

void Session::HandleAlert(uv_async_t* handle)
{
    dispatch_data* data = static_cast<dispatch_data*>(handle->data);
}

