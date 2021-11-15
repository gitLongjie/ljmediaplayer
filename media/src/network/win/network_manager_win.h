#ifndef src_network_win_network_manager_win_h_
#define src_network_win_network_manager_win_h_

#include "src/network/network_manager_std.h"

namespace LJMP {
    namespace Network {
        class NetworkManagerWin : public NetworkManagerStd {
            disable_copy(NetworkManagerWin)

        public:
            static NetworkManagerPtr create(TaskQueuePtr taskQueue);

        public:
            ~NetworkManagerWin() override;

        protected:
            explicit NetworkManagerWin(TaskQueuePtr taskQueue);

            void doInitialize(NetworkManagerWPtr wThis);
            void doUninitialize(NetworkManagerWPtr wThis);
        };
    }
}

#endif // !src_network_win_network_manager_win_h_
