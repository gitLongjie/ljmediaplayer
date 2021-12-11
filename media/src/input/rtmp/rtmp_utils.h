#ifndef src_input_rtmp_rtmp_utils_h_
#define src_input_rtmp_rtmp_utils_h_

#include <string>

#include "src/lj_defined.h"


namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class RtmpUtils {
                disable_copy(RtmpUtils)
            public:
                RtmpUtils() = delete;
                ~RtmpUtils() = delete;

                static double getAFMMetaDataByName(const char* meta_data, unsigned int len,
                    const char* name, unsigned int name_lenght);
            };
        }
    }
}

#endif // !src_input_rtmp_utils_h_

