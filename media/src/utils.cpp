#include "src/utils.h"

#include <string>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#include <io.h>

#else
#include <unistd.h>
#include <fcntl.h>

#define Access
#endif

static std::string emptyStr;

namespace LJMP {
    bool Utils::isFileExits(const char *szPath) {
        if ( 0 == access(szPath, 0)) {
            return true;
        } else {
            return false;
        }
    }

    bool Utils::checkProtocol(const std::string& url, const char* protocol) {
        std::string sProtoc = getProtocol(url);
        return 0 == sProtoc.compare(protocol);
    }

    bool Utils::checkProtocol(const std::string& url, const StringList& protocols) {
        std::string protocol = getProtocol(url);
        auto itor = std::find(protocols.begin(), protocols.end(), protocol);
        if (protocols.end() == itor) {
            return false;
        }
        return true;
    }

    std::string Utils::getProtocol(const std::string& url) {
        size_t pos = url.find("://");
        if (std::string::npos == pos) {
            return "";
        }

        std::string protocol = url.substr(0, pos + 3);
        return protocol;
    }

    std::vector<std::string> Utils::split(const std::string& str, const std::string& sp,
        bool ignore_empty, bool split_empty) {
        struct {
            auto len(const std::string& s) { return s.length(); }
            auto len(const std::string::value_type* p) {
                return p ? std::char_traits<std::string::value_type>::length(p) : 0;
            }
            auto len(const std::string::value_type  c) { return c == std::string::value_type() ? 0 : 1; }
        } util;

        if (str.empty()) { /// empty string ///
            if (!split_empty || util.len(sp)) return { "" };
            return {};
        }

        auto v = std::vector<std::string>();
        auto n = static_cast<std::string::size_type>(util.len(sp));
        if (n == 0) {  
            if (!split_empty) return { str };
            for (auto&& c : str) v.emplace_back(1, c);
            return v;
        }

        auto p = std::string::size_type(0);
        while (1) { 
            auto pos = str.find(sp, p);
            if (pos == std::string::npos) {
                if (ignore_empty && p - n + 1 == str.size()) break;
                v.emplace_back(str.begin() + p, str.end());
                break;
            }
            if (!ignore_empty || p != pos)
                v.emplace_back(str.begin() + p, str.begin() + pos);
            p = pos + n;
        }
        return v;
    }

    unsigned int Utils::getTime() {
#if WIN32
        return timeGetTime();
#else
        struct tms t;
        if (!clk_tck) clk_tck = sysconf(_SC_CLK_TCK);
        return times(&t) * 1000 / clk_tck;
#endif
    }

    const std::string& Utils::emptyString() {
        return emptyStr;
    }

} // namespace LJMP
