/* Copyright (c) 2016 Sergiy Zhukovs'kyy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <node_buffer.h>
#include "AWParserWrap.h"

namespace AWParserWrap {

    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::MaybeLocal;
    using v8::Int32;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Boolean;
    using v8::Value;
    using v8::Exception;
    using v8::NewStringType;

    char *deserializedData = nullptr;

    Persistent<Function> CAWParserWrap::constructor;

    CAWParserWrap::CAWParserWrap() {
    }

    CAWParserWrap::~CAWParserWrap() {
    }

    void CAWParserWrap::Init(Local<Object> exports) {
        Isolate* isolate = exports->GetIsolate();

        // Prepare constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        tpl->SetClassName(String::NewFromUtf8(isolate, "CAWParser", NewStringType::kNormal).ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(tpl, "addHost", CAWParserWrap::AddHost);
        NODE_SET_PROTOTYPE_METHOD(tpl, "matchesHost", CAWParserWrap::MatchesHost);
        NODE_SET_PROTOTYPE_METHOD(tpl, "serialize", CAWParserWrap::Serialize);
        NODE_SET_PROTOTYPE_METHOD(tpl, "deserialize", CAWParserWrap::Deserialize);
        NODE_SET_PROTOTYPE_METHOD(tpl, "cleanup", CAWParserWrap::Cleanup);

        constructor.Reset(isolate, tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
        exports->Set(String::NewFromUtf8(isolate, "CAWParser", NewStringType::kNormal).ToLocalChecked(),
                     tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
    }

    void CAWParserWrap::New(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.IsConstructCall()) {
            // Invoked as constructor: `new CAWParserWrap(...)`
            CAWParserWrap* obj = new CAWParserWrap();
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // Invoked as plain function `CAWParserWrap(...)`,
            // turn into construct call.
            const int argc = 1;
            Local<Value> argv[argc] = { args[0] };
            Local<Function> cons = Local<Function>::New(isolate, constructor);
            args.GetReturnValue().Set(
                cons->NewInstance(isolate->GetCurrentContext(), argc, argv)
                    .ToLocalChecked());
        }
    }

    void CAWParserWrap::AddHost(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        if (args.Length() != 1) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Wrong number of arguments", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        if (!args[0]->IsString()) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Wrong arguments", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext()).FromMaybe(Local<String>()));
        const char * buffer = *str;

        CAWParserWrap* obj = ObjectWrap::Unwrap<CAWParserWrap>(args.Holder());
        obj->addHost(buffer);
    }

    void CAWParserWrap::MatchesHost(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        if (args.Length() != 1) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Wrong number of arguments", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        if (!args[0]->IsString()) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Wrong arguments", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        String::Utf8Value strHost(isolate, args[0]->ToString(isolate->GetCurrentContext()).FromMaybe(Local<String>()));
        const char * bufferHost = *strHost;

        CAWParserWrap* obj = ObjectWrap::Unwrap<CAWParserWrap>(args.Holder());
        args.GetReturnValue().Set(Boolean::New(isolate, obj->matchesHost(bufferHost)));
    }

    void CAWParserWrap::Serialize(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        CAWParserWrap* obj = ObjectWrap::Unwrap<CAWParserWrap>(args.Holder());

        unsigned int totalSize = 0;
        // Serialize data
        char* data = obj->serialize(&totalSize);
        if (nullptr == data) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Could not serialize", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        MaybeLocal<Object> buffer = node::Buffer::New(isolate, totalSize);
        Local<Object> localBuffer;
        if (!buffer.ToLocal(&localBuffer)) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Could not convert MaybeLocal to Local", NewStringType::kNormal).ToLocalChecked()));
            return;
        }
        ::memcpy(node::Buffer::Data(localBuffer), data, totalSize);

        delete []data;

        args.GetReturnValue().Set(localBuffer);
    }

    void CAWParserWrap::Deserialize(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        if (args.Length() < 1) {
            isolate->ThrowException(Exception::TypeError(
                                                         String::NewFromUtf8(isolate, "Wrong number of arguments", NewStringType::kNormal).ToLocalChecked()));
            return;
        }

        unsigned char *buf = (unsigned char *)node::Buffer::Data(args[0]);
        size_t length = node::Buffer::Length(args[0]);

        if (nullptr != deserializedData) {
            delete []deserializedData;
        }
        deserializedData = new char[length];
        memcpy(deserializedData, buf, length);

        CAWParserWrap* obj = ObjectWrap::Unwrap<CAWParserWrap>(args.Holder());
        args.GetReturnValue().Set(Boolean::New(isolate, obj->deserialize(deserializedData)));
    }

    void CAWParserWrap::Cleanup(const FunctionCallbackInfo<Value>& args) {
        if (nullptr != deserializedData) {
            delete []deserializedData;
            deserializedData = nullptr;
        }
    }

}   //namespace AWParserWrap
