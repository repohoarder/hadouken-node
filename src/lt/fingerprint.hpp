#ifndef _HDKN_LT_FINGERPRINT_HPP
#define _HDKN_LT_FINGERPRINT_HPP

#include <node.h>
#include <env.h>
#include <v8.h>
#include <libtorrent/fingerprint.hpp>

namespace lt
{
    class Fingerprint : public node::BaseObject
    {
    public:
        Fingerprint(node::Environment* env, v8::Local<v8::Object> wrap)
            : BaseObject(env, wrap),
            initialized_(false),
            fp_(0)
        {
        }

        ~Fingerprint() override
        {
            if (fp_ != nullptr)
            {
                delete fp_;
            }
        }

        static void Initialize(node::Environment* env, v8::Handle<v8::Object> exports);
        
        libtorrent::fingerprint* fp_;

    protected:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        
        static void GetName(const v8::FunctionCallbackInfo<v8::Value>& args);

        bool Init(std::string id, int major, int minor, int revision, int tag);

    private:
        bool initialized_;
    };
}

#endif
