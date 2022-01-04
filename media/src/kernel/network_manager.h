#ifndef src_network_manager_h_
#define src_network_manager_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/kernel/io_event.h"
#include "src/kernel/channel.h"

namespace LJMP {
    class INetworkManager {
    public:
        using Ptr = std::shared_ptr<INetworkManager>;
        using WPtr = std::weak_ptr<INetworkManager>;

    public:
        virtual ~INetworkManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;

        virtual void updateChannel(const IChannel::Ptr& channel, unsigned int  io_event) = 0;
        virtual void removeChannel(const IChannel::Ptr& channel) = 0;
        virtual void addConnectChannel(const IChannel::Ptr& channel) = 0;
    };
}

#endif
