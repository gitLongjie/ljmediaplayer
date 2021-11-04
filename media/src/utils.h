//
//  utils.h
//  ljplayer
//
//  Created by Tal on 2021/11/3.
//

#ifndef src_utils_h_
#define src_utils_h_

#include "src/lj_defined.h"

namespace LJMP {
    class Utils {
        disable_copy(Utils)
        Utils() = delete;
        ~Utils() = delete;
        
    public:
        bool isFileExits(const char* url);
    };
}

#endif /* src_utils_h_ */
