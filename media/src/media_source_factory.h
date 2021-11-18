#ifndef src_media_source_factory_h_
#define src_media_source_factory_h_

#include <memory>

#include "src/media_source.h"

namespace LJMP {

    class MediaSourceFactory {
    public:
        MediaSourceFactory() = default;
        virtual ~MediaSourceFactory() = default;

        virtual MediaSourcePtr create() = 0;
        virtual bool isSupportProtocol(const std::string& protocol) const = 0;
    };
    using MediaSourceFactoryPtr = std::shared_ptr<MediaSourceFactory>;

    typedef bool(*checkProtocol)(const std::string&);
    template <class T, checkProtocol CP>
    class MediaSourceFactoryImpl : public MediaSourceFactory {
        disable_copy(MediaSourceFactoryImpl)

    public:
        MediaSourceFactoryImpl() = default;
        ~MediaSourceFactoryImpl() override = default;

        MediaSourcePtr create() override {
            struct Creator : public T {
                Creator() : T() {}
                ~Creator() override = default;
            };
            return std::move(std::make_shared<Creator>());
        }

        bool isSupportProtocol(const std::string& protocol) const override {
            return CP(protocol);
        }
    };
}

#endif // !src_media_source_factory_h_
