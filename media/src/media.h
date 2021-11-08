#ifndef src_media_h_
#define src_media_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class Log;
    class MediaSourceManager;

    class Media {
        disable_copy(Media)

    public:
        Media();
        ~Media();
        
        bool initialize();
        void uninialize();
        
    public:
        bool openUrl(const char* szUrl);

    public:
        static Media* getInstance();
        
    private:
        std::shared_ptr<Log> log_;
        std::shared_ptr<MediaSourceManager> input_media_source_manager_;
    };
}

#endif  // src_media_h_
