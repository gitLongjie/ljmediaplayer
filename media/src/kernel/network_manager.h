#ifndef src_network_manager_h_
#define src_network_manager_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class INetworkManager {
    public:
        using Ptr = std::shared_ptr<INetworkManager>;
        using WPtr = std::weak_ptr<INetworkManager>;

    public:
        virtual ~INetworkManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;
    };
}

#endif
