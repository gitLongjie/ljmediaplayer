#ifndef src_media_h_
#define src_media_h_

#include "src/lj_defined.h"

namespace LJMP {
    class Media {
        disable_copy(Media)

    public:
        Media();
        ~Media();
        
    public:
        bool openUrl(const char* szUrl);

    public:
        static Media* getInstance();
    };
}

#endif  // src_media_h_
