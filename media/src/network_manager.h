#ifndef src_network_manager_h_
#define src_network_manager_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class NetworkManager : public std::enable_shared_from_this<NetworkManager> {
    public:
        virtual ~NetworkManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;
    };

    using NetworkManagerPtr = std::shared_ptr<NetworkManager>;
    using NetworkManagerWPtr = std::weak_ptr<NetworkManager>;
}

#endif
