#include "src/input/rtmp/rtmp_utils.h"

#include "src/log.h"
#include "src/network/network_utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            void RtmpUtils::amfAddProp(AMFObject* obj, const AMFObjectProperty* prop) {
                if (!(obj->o_num & 0x0f)) {
                    obj->o_props =
                        (AMFObjectProperty*)realloc(obj->o_props, (obj->o_num + 16) * sizeof(AMFObjectProperty));
                }
                    
                memcpy(&obj->o_props[obj->o_num++], prop, sizeof(AMFObjectProperty));
            }

            char* RtmpUtils::amfEncodeString(char* output, char* outend, const std::string& str) {
                if ((str.length() < 65536 && output + 1 + 2 + str.length() > outend) ||
                    output + 1 + 4 + str.length() > outend) {
                    return nullptr;
                }

                if (str.length() < 65536) {
                    *output++ = AMF_STRING;

                    output = amfEncodeInt16(output, outend, static_cast<short>(str.length()));
                }
                else {
                    *output++ = AMF_LONG_STRING;

                    output = amfEncodeInt32(output, outend, static_cast<int>(str.length()));
                }
                memcpy(output, str.c_str(), str.length());
                output += str.length();

                return output;
            }

            char* RtmpUtils::amfEncodeNumber(char* output, char* outend, double dVal) {
                if (output + 1 + 8 > outend)
                    return nullptr;

                *output++ = AMF_NUMBER;	/* type: Number */

#if __FLOAT_WORD_ORDER == __BYTE_ORDER
#if __BYTE_ORDER == __BIG_ENDIAN
                memcpy(output, &dVal, 8);
#elif __BYTE_ORDER == __LITTLE_ENDIAN
                {
                    unsigned char* ci, * co;
                    ci = (unsigned char*)&dVal;
                    co = (unsigned char*)output;
                    co[0] = ci[7];
                    co[1] = ci[6];
                    co[2] = ci[5];
                    co[3] = ci[4];
                    co[4] = ci[3];
                    co[5] = ci[2];
                    co[6] = ci[1];
                    co[7] = ci[0];
                }
#endif
#else
#if __BYTE_ORDER == __LITTLE_ENDIAN	/* __FLOAT_WORD_ORER == __BIG_ENDIAN */
                {
                    unsigned char* ci, * co;
                    ci = (unsigned char*)&dVal;
                    co = (unsigned char*)output;
                    co[0] = ci[3];
                    co[1] = ci[2];
                    co[2] = ci[1];
                    co[3] = ci[0];
                    co[4] = ci[7];
                    co[5] = ci[6];
                    co[6] = ci[5];
                    co[7] = ci[4];
                }
#else /* __BYTE_ORDER == __BIG_ENDIAN && __FLOAT_WORD_ORER == __LITTLE_ENDIAN */
                {
                    unsigned char* ci, * co;
                    ci = (unsigned char*)&dVal;
                    co = (unsigned char*)output;
                    co[0] = ci[4];
                    co[1] = ci[5];
                    co[2] = ci[6];
                    co[3] = ci[7];
                    co[4] = ci[0];
                    co[5] = ci[1];
                    co[6] = ci[2];
                    co[7] = ci[3];
                }
#endif
#endif

                return output + 8;
            }

            char* RtmpUtils::amfEncodeInt16(char* output, char* outend, short nVal) {
                if (output + 2 > outend)
                    return nullptr;

                output[1] = nVal & 0xff;
                output[0] = nVal >> 8;
                return output + 2;
            }

            char* RtmpUtils::amfEncodeInt24(char* output, char* outend, int nVal) {
                if (output + 3 > outend)
                    return nullptr;

                output[2] = nVal & 0xff;
                output[1] = nVal >> 8;
                output[0] = nVal >> 16;
                return output + 3;
            }

            char* RtmpUtils::amfEncodeInt32(char* output, char* outend, int nVal) {
                if (output + 4 > outend)
                    return nullptr;

                output[3] = nVal & 0xff;
                output[2] = nVal >> 8;
                output[1] = nVal >> 16;
                output[0] = nVal >> 24;
                return output + 4;
            }

            char* RtmpUtils::amfEncodeBoolean(char* output, char* outend, int bVal) {
                if (output + 2 > outend)
                    return nullptr;

                *output++ = AMF_BOOLEAN;

                *output++ = bVal ? 0x01 : 0x00;

                return output;
            }

            char* RtmpUtils::amfEncodeNamedString(char* output, char* outend, const std::string& name, const std::string& value) {
                if (output + 2 + name.length() > outend)
                    return NULL;
                output = amfEncodeInt16(output, outend, static_cast<short>(name.length()));

                memcpy(output, name.c_str(), name.length());
                output += name.length();

                return amfEncodeString(output, outend, value);
            }

            char* RtmpUtils::amfEncodeNamedNumber(char* output, char* outend, const std::string& name, double dVal) {
                if (output + 2 + name.length() > outend)
                    return nullptr;
                output = amfEncodeInt16(output, outend, static_cast<short>(name.length()));

                memcpy(output, name.c_str(), name.length());
                output += name.length();

                return amfEncodeNumber(output, outend, dVal);
            }

            char* RtmpUtils::amfEncodeNamedBoolean(char* output, char* outend, const std::string& name, int bVal) {
                if (output + 2 + name.length() > outend)
                    return nullptr;
                output = amfEncodeInt16(output, outend, static_cast<short>(name.length()));

                memcpy(output, name.c_str(), name.length());
                output += name.length();

                return amfEncodeBoolean(output, outend, bVal);
            }

        }
    }
}