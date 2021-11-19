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
            static NetworkManagerPtr create(TaskQueuePtr task_queue);

        public:
            ~NetworkManagerStd() override;

            bool initialize() override;
            void uninitialize() override;

            void invoke(const TaskPtr task);
            void invoke(const TaskPtr task, uint16_t delay);

            void updateChannel(const std::shared_ptr<Channel>& channel);
            void removeChannel(const std::shared_ptr<Channel>& channel);

        protected:
            explicit NetworkManagerStd(TaskQueuePtr taskQueue);

            virtual void doInitialize(NetworkManagerWPtr wThis);
            virtual void doUninitialize(NetworkManagerWPtr wThis);

            void doUpdateChannel(const std::shared_ptr<Channel>& channel, NetworkManagerWPtr wThis);
            void doRemoveChannel(const std::shared_ptr<Channel>& channel, NetworkManagerWPtr wThis);

            void select();
            void doSelect(NetworkManagerWPtr wThis);

        private:
            TaskQueuePtr io_task_queue_;
            SpinLock spin_lock_;
            bool stop_ = true;

            using ChannelList = std::map<socket_t, std::shared_ptr<Channel>>;
            ChannelList channels_;
        };
        using NetworkManagerStdPtr = std::shared_ptr<NetworkManagerStd>;
    }
}

#endif // !src_network_network_manager_std_h_

