#ifndef src_network_network_manager_h_
#define src_network_network_manager_h_

#include <memory>
#include "src/lj_defined.h"

#include "src/network_manager.h"
#include "src/task_queue.h"

namespace LJMP {
    namespace Network {
        class NetworkManagerStd : public NetworkManager {
            disable_copy(NetworkManagerStd)

        public:
            static NetworkManagerPtr create(TaskQueuePtr taskQueue);

        public:
            ~NetworkManagerStd() override;

            bool initialize() override;
            void uninitialize() override;

            void invoke(const TaskPtr task);
            void invoke(const TaskPtr task, uint16_t delay);

        protected:
            explicit NetworkManagerStd(TaskQueuePtr taskQueue);

            virtual void doInitialize(NetworkManagerWPtr wThis);
            virtual void doUninitialize(NetworkManagerWPtr wThis);

        private:
            TaskQueuePtr io_task_queue_;
        };
    }
}

#endif // !src_network_network_manager_h_

