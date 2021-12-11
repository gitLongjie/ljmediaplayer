#ifndef src_network_manager_h_
#define src_network_manager_h_

#include <memory>
#include <list>
#include <string>

#include "src/lj_defined.h"

#include "src/task_queue_object.h"

namespace LJMP {
    class NetworkManager : public TaskQueueObject {
    public:
        using Ptr = std::shared_ptr<NetworkManager>;

    public:
        virtual ~NetworkManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;

    protected:
        explicit NetworkManager(const TaskQueue::Ptr& task_queue) : TaskQueueObject(task_queue, false) {}
    };
}

#endif
