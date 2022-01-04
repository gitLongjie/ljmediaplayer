#ifndef src_network_network_manager_std_h_
#define src_network_network_manager_std_h_

#include <memory>
#include <map>

#include "src/lj_defined.h"

#include "src/core/task_object.h"
#include "src/core/spin_lock.h"
#include "src/core/task_queue.h"

#include "src/kernel/network_manager.h"
#include "src/network/lj_network_define.h"

namespace LJMP {
    namespace Network {

        class NetworkManagerStd : public TaskObject, public INetworkManager {
            disable_copy(NetworkManagerStd)

        public:
            static INetworkManager::Ptr create(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

        public:
            ~NetworkManagerStd() override;

            bool initialize() override;
            void uninitialize() override;

            void updateChannel(const IChannel::Ptr& channel, unsigned int io_event) override;
            void removeChannel(const IChannel::Ptr& channel) override;
            void addConnectChannel(const IChannel::Ptr& channel) override;

        protected:
            NetworkManagerStd(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue);

            virtual void doInitialize(ObjectPtr::WPtr wThis);
            virtual void doUninitialize(ObjectPtr::WPtr wThis);

            void doUpdateChannel(const IChannel::Ptr& channel, unsigned int io_event, ObjectPtr::WPtr wThis);
            void doRemoveChannel(const IChannel::Ptr& channel, ObjectPtr::WPtr wThis);
            void doAddConnectChannel(const IChannel::Ptr& channel, ObjectPtr::WPtr wThis);

        private:
            SpinLock spin_lock_;
            bool stop_ = true;

            IIOEvent::Ptr io_event_;
        };
        
    }
}

#endif // !src_network_network_manager_std_h_

