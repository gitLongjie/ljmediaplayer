#include "src/input/rtmp/rtmp_utils.h"

#include "src/input/rtmp/amf.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            double RtmpUtils::getAFMMetaDataByName(const char* meta_data, unsigned int len,
                const char* name, unsigned int name_lenght) {
                std::string data(meta_data, len);
                size_t pos = data.find(name);
                if (std::string::npos == pos) {
                    return -1.0;
                }

                pos += name_lenght;
                if (pos + 8 >= len) {
                    return -1.0;
                }

                return AMF_DecodeNumber(meta_data + pos);
            }

        }
    }
}