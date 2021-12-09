#ifndef src_lj_defined_h_
#define src_lj_defined_h_

#include <vector>
#include <string>

using StringList = std::vector<std::string>;

#define disable_copy(class_name)                            \
   class_name(const class_name& ) = delete;                 \
   class_name operator= (const class_name& ) = delete;      

#define implate_creator(class_name)                         \
    struct Creator : public NetworkManager {                \
        Creator() : NetworkManager() {}                     \
        ~Creator() override = default;                      \
    };

#define return_implate_creater()                            \
    return std::make_shared<Creator>();

template <typename T, typename V>
inline void invokeCallback(T func, V val) {
    if (func) {
        func(val);
    }
}

enum class CodecType {
    Audio_AAC,

    Video_X264,
};


#endif // ! src_lj_defined_h_