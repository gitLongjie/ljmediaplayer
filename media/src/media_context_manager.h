#ifndef src_media_context_manager_h_
#define src_media_context_manager_h_

#include <memory>
#include <string>
#include <map>

#include "src/lj_defined.h"

namespace LJMP {
    class MediaContext;

    class MediaContextManager : public std::enable_shared_from_this<MediaContextManager> {
        disable_copy(MediaContextManager)

    public:
        using Ptr = std::shared_ptr<MediaContextManager>;
        using WPtr = std::weak_ptr<MediaContextManager>;

        static Ptr create();
    public:
        virtual ~MediaContextManager() = default;

        bool isContain(const std::string& url) const {
            return media_context_url_lists_.end() != media_context_url_lists_.find(url);
        }

        bool addMediaContext(const std::shared_ptr<MediaContext>& media_contxt, const std::string& url);
        void removeMediaContext(const std::string& url);

        void destroy();


    protected:
        MediaContextManager() = default;

    private:
        using MediaContextUrlLists = std::map<std::string, std::shared_ptr<MediaContext> >;
        MediaContextUrlLists media_context_url_lists_;
    };
}

#endif // !src_media_context_manager_h_

