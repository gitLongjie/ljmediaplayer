//
//  utils.h
//  ljplayer
//
//  Created by Tal on 2021/11/3.
//

#ifndef src_utils_h_
#define src_utils_h_

#include "src/lj_defined.h"

#include <vector>
#include <string>

namespace LJMP {
    class Utils {
        disable_copy(Utils)
        Utils() = delete;
        ~Utils() = delete;
        
    public:
        static bool isFileExits(const char* url);
        static bool checkProtocol(const std::string& ur, const char* protocol);
        static bool checkProtocol(const std::string& ur, const StringList& protocols);
        static std::string getProtocol(const std::string& url);
        static std::vector<std::string> split(const std::string& str, const std::string& sp,
            bool ignore_empty = 0, bool split_empty = 0);
    };
}

#endif /* src_utils_h_ */
