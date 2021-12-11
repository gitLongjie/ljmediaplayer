#ifndef src_network_network_manager_std_h_
#define src_network_network_manager_std_h_

#include <memory>
#include <map>

#include "src/lj_defined.h"

#include "src/network_manager.h"
#include "src/spin_lock.h"
#include "src/task_queue.h"
#include "src/network/lj_network_define.h"

namespace LJMP {
    namespace Network {
        class Channel;

        class NetworkManagerStd : public NetworkManager {
            disable_copy(NetworkManagerStd)

        public:
            static NetworkManagerStd::Ptr create(const TaskQueue::Ptr& task_queue);

        public:
            ~NetworkManagerStd() override;

            bool initialize() override;
            void uninitialize() override;

            void updateChannel(const std::shared_ptr<Channel>& channel);
            void removeChannel(const std::shared_ptr<Channel>& channel);

        protected:
            explicit NetworkManagerStd(const TaskQueue::Ptr& taskQueue);

            virtual void doInitialize(WPtr wThis);
            virtual void doUninitialize(WPtr wThis);

            void doUpdateChannel(const std::shared_ptr<Channel>& channel, WPtr wThis);
            void doRemoveChannel(const std::shared_ptr<Channel>& channel, WPtr wThis);

            void select(unsigned long long dely);
            void doSelect(WPtr wThis);

        private:
            SpinLock spin_lock_;
            bool stop_ = true;

            using ChannelList = std::map<socket_t, std::shared_ptr<Channel>>;
            ChannelList channels_;
        };
        
    }
}

#endif // !src_network_network_manager_std_h_

