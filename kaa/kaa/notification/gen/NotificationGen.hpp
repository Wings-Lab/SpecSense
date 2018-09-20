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


#ifndef KAA_NOTIFICATION_GEN_NOTIFICATIONGEN_HPP_1210959396__H_
#define KAA_NOTIFICATION_GEN_NOTIFICATIONGEN_HPP_1210959396__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace kaa_notification {
enum power_iq_change_choice {
    Y_change,
    N_change,
};

struct ConfigurationChange5 {
    power_iq_change_choice power_iq_change;
    std::string NewSamplingFrequency;
    std::string NewNumberOFBins;
    std::string NewNoAvgdSpectra;
    ConfigurationChange5() :
        power_iq_change(power_iq_change_choice()),
        NewSamplingFrequency(std::string()),
        NewNumberOFBins(std::string()),
        NewNoAvgdSpectra(std::string())
        { }
};

}
namespace avro {
template<> struct codec_traits<kaa_notification::power_iq_change_choice> {
    static void encode(Encoder& e, kaa_notification::power_iq_change_choice v) {
		if (v < kaa_notification::Y_change || v > kaa_notification::N_change)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for kaa_notification::power_iq_change_choice and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, kaa_notification::power_iq_change_choice& v) {
		size_t index = d.decodeEnum();
		if (index < kaa_notification::Y_change || index > kaa_notification::N_change)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for kaa_notification::power_iq_change_choice and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<kaa_notification::power_iq_change_choice>(index);
    }
};

template<> struct codec_traits<kaa_notification::ConfigurationChange5> {
    static void encode(Encoder& e, const kaa_notification::ConfigurationChange5& v) {
        avro::encode(e, v.power_iq_change);
        avro::encode(e, v.NewSamplingFrequency);
        avro::encode(e, v.NewNumberOFBins);
        avro::encode(e, v.NewNoAvgdSpectra);
    }
    static void decode(Decoder& d, kaa_notification::ConfigurationChange5& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.power_iq_change);
                    break;
                case 1:
                    avro::decode(d, v.NewSamplingFrequency);
                    break;
                case 2:
                    avro::decode(d, v.NewNumberOFBins);
                    break;
                case 3:
                    avro::decode(d, v.NewNoAvgdSpectra);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.power_iq_change);
            avro::decode(d, v.NewSamplingFrequency);
            avro::decode(d, v.NewNumberOFBins);
            avro::decode(d, v.NewNoAvgdSpectra);
        }
    }
};

}
#endif
