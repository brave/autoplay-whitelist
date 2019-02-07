/* Copyright (c) 2016 Sergiy Zhukovs'kyy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AWPARSERWRAP_H_
#define AWPARSERWRAP_H_

#include <node.h>
#include <node_object_wrap.h>

#include "../autoplay_whitelist_parser.h"

namespace AWParserWrap {

    /**
     * Wraps Autoplay Whitelist for use in Node
     */
    class CAWParserWrap : public AutoplayWhitelistParser, public node::ObjectWrap {
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        CAWParserWrap();
        virtual ~CAWParserWrap();

        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void AddHost(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void MatchesHost(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Serialize(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Deserialize(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Cleanup(const v8::FunctionCallbackInfo<v8::Value>& args);

        static v8::Persistent<v8::Function> constructor;
    };

}   // namespace AWParserWrap

#endif  //AWPARSERWRAP_H_
