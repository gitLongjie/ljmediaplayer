#ifndef src_network_win_network_manager_win_h_
#define src_network_win_network_manager_win_h_

#include "src/network/network_manager_std.h"

namespace LJMP {
    namespace Network {
        class NetworkManagerWin : public NetworkManagerStd {
            disable_copy(NetworkManagerWin)

        public:
            static INetworkManager::Ptr create(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

        public:
            ~NetworkManagerWin() override;

        protected:
            NetworkManagerWin(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

            void doInitialize(ObjectPtr::WPtr wThis);
            void doUninitialize(ObjectPtr::WPtr wThis);
        };
    }
}

#endif // !src_network_win_network_manager_win_h_
