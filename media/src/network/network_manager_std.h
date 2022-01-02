#ifndef src_network_network_manager_std_h_
#define src_network_network_manager_std_h_

#include <memory>
#include <map>

#include "src/lj_defined.h"

#include "src/core/task_object.h"
#include "src/core/spin_lock.h"
#include "src/core/task_queue.h"

#include "src/kernel/io_event.h"
#include "src/kernel/network_manager.h"
#include "src/network/lj_network_define.h"

namespace LJMP {
    namespace Network {
        class Channel;

        class NetworkManagerStd : public TaskObject, public INetworkManager {
            disable_copy(NetworkManagerStd)

        public:
            static INetworkManager::Ptr create(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

        public:
            ~NetworkManagerStd() override;

            bool initialize() override;
            void uninitialize() override;

            void updateChannel(const std::shared_ptr<Channel>& channel);
            void removeChannel(const std::shared_ptr<Channel>& channel);
            void addConnectChannel(const std::shared_ptr<Channel>& channel);

        protected:
            NetworkManagerStd(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

            virtual void doInitialize(ObjectPtr::WPtr wThis);
            virtual void doUninitialize(ObjectPtr::WPtr wThis);

            void doUpdateChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis);
            void doRemoveChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis);
            void doAddConnectChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis);

        private:
            SpinLock spin_lock_;
            bool stop_ = true;

            IIOEvent::Ptr io_event_;

            using ChannelList = std::map<socket_t, std::shared_ptr<Channel>>;
            ChannelList channels_;

            ChannelList connect_channels_;
        };
        
    }
}

#endif // !src_network_network_manager_std_h_

