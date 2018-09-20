/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef KAA_CONFIGURATION_GEN_CONFIGURATIONGEN_HPP_1210959396__H_
#define KAA_CONFIGURATION_GEN_CONFIGURATIONGEN_HPP_1210959396__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace kaa_configuration {
struct EmptyData {
    EmptyData()
        { }
};

}
namespace avro {
template<> struct codec_traits<kaa_configuration::EmptyData> {
    static void encode(Encoder& e, const kaa_configuration::EmptyData& v) {
    }
    static void decode(Decoder& d, kaa_configuration::EmptyData& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                default:
                    break;
                }
            }
        } else {
        }
    }
};

}
#endif
