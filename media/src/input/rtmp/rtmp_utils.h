#ifndef src_input_rtmp_rtmp_utils_h_
#define src_input_rtmp_rtmp_utils_h_

#include <string>

#include "src/lj_defined.h"

#include "src/input/rtmp/rtmp_define.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class RtmpUtils {
                disable_copy(RtmpUtils)
            public:
                RtmpUtils() = delete;
                ~RtmpUtils() = delete;

                static void amfAddProp(AMFObject* obj, const AMFObjectProperty* prop);
                static char* amfEncodeString(char* output, char* outend, const std::string& str);
                static char* amfEncodeString(char* output, char* outend, const AVal* str) {
                    return amfEncodeString(output, outend, std::string(str->av_val, str->av_len));
                }
                static char* amfEncodeNumber(char* output, char* outend, double dVal);
                static char* amfEncodeInt16(char* output, char* outend, short nVal);
                static char* amfEncodeInt24(char* output, char* outend, int nVal);
                static char* amfEncodeInt32(char* output, char* outend, int nVal);
                static char* amfEncodeBoolean(char* output, char* outend, int bVal);

                static char* amfEncodeNamedString(char* output, char* outend, const std::string& name,
                    const std::string& value);
                static char* amfEncodeNamedString(char* output, char* outend, const AVal* name,
                    const std::string& value) {
                    return amfEncodeNamedString(output, outend, std::string(name->av_val, name->av_len),value);
                }
                static char* amfEncodeNamedString(char* output, char* outend, const AVal* name, const AVal* value) {
                    return amfEncodeNamedString(output, outend, std::string(name->av_val, name->av_len),
                        std::string(value->av_val, value->av_len));
                }
                static char* amfEncodeNamedNumber(char* output, char* outend, const AVal* name, double dVal);
                static char* amfEncodeNamedBoolean(char* output, char* outend, const AVal* name, int bVal);

                static std::string avalToString(const AVal& av);

                static char* amfEncode(AMFObject* obj, char* pBuffer, char* pBufEnd);
                static char* amfEncodeEcmaArray(AMFObject* obj, char* pBuffer, char* pBufEnd);
                static char* amfEncodeArray(AMFObject* obj, char* pBuffer, char* pBufEnd);
                static char* amfPropEncode(AMFObjectProperty* prop, char* buffer, char* buffer_end);
            };
        }
    }
}

#endif // !src_input_rtmp_utils_h_

