#ifndef src_lj_defined_h_
#define src_lj_defined_h_

#include <memory>
#include <vector>
#include <string>

using FD = unsigned long long;

using StringList = std::vector<std::string>;

#define disable_copy(class_name)                            \
   class_name(const class_name& ) = delete;                 \
   class_name operator= (const class_name& ) = delete;      

template <class T, typename... Args>
std::shared_ptr<T> createPtr(Args&&... args) {
    struct Creator : public T {
        Creator(Args&&... args) : T(std::forward<Args>(args)...) {}
        ~Creator() override {}
    };

    return std::make_shared<Creator>(std::forward<Args>(args)...);
}

template <typename T, typename V>
inline void invokeCallback(T func, V val) {
    if (func) {
        func(val);
    }
}

enum class DataType {
    Script,
    Audio,
    Video
};

enum class CodecType {
    Unknown,
    Audio_AAC,

    Video_X264,
    Video_FFMpeg_Decode,
};

struct MediaConfig {
    int audio_sample;
    int audio_channel;
    int audio_codec_id;

    int video_width;
    int video_height;
    int video_frame;
    int vidoe_codec_id;
};

#endif // ! src_lj_defined_h_