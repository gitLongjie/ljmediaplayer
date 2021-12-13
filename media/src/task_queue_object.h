#ifndef src_task_queue_object_h
#define src_task_queue_object_h

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue.h"
#include "src/spin_lock.h"

namespace LJMP {
    class TaskQueueObjectImpl;

    class TaskQueueObject : public std::enable_shared_from_this<TaskQueueObject> {
        disable_copy(TaskQueueObject)

    public:
        using Ptr = std::shared_ptr<TaskQueueObject>;
        using WPtr = std::weak_ptr<TaskQueueObject>;

    public:
        virtual ~TaskQueueObject();
        void destory();

    protected:
        TaskQueueObject(const TaskQueue::Ptr& task_queue, bool own_thread);

        bool invoke(const Task::Ptr& task);
        bool invoke(const Task::Ptr& task, uint16_t delay);
        bool isCurrentThread();

        TaskQueue::Ptr getTaskQueue() const;

    protected:
        virtual void onDestory();

    private:
        void onDestroyImpl(WPtr wThis);

    private:
        std::unique_ptr<TaskQueueObjectImpl> impl_;
        SpinLock spink_lock_;
        
    };
}

#endif // !src_task_queue_object_h

