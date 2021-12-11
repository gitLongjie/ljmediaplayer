#ifndef src_network_win_network_manager_win_h_
#define src_network_win_network_manager_win_h_

#include "src/network/network_manager_std.h"

namespace LJMP {
    namespace Network {
        class NetworkManagerWin : public NetworkManagerStd {
            disable_copy(NetworkManagerWin)

        public:
            static NetworkManager::Ptr create(const TaskQueue::Ptr& task_queue);

        public:
            ~NetworkManagerWin() override;

        protected:
            explicit NetworkManagerWin(const TaskQueue::Ptr& task_queue);

            void doInitialize(WPtr wThis);
            void doUninitialize(WPtr wThis);
        };
    }
}

#endif // !src_network_win_network_manager_win_h_
