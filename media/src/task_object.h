#ifndef src_task_object_h_
#define src_task_object_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue.h"
#include "src/spin_lock.h"

namespace LJMP {
    class TaskObjectImpl;

    class TaskObject : public std::enable_shared_from_this<TaskObject> {
        disable_copy(TaskObject)

    public:
        using Ptr = std::shared_ptr<TaskObject>;
        using WPtr = std::weak_ptr<TaskObject>;

    public:
        virtual ~TaskObject();
        void destory();

    protected:
        TaskObject(const TaskQueue::Ptr& task_queue, bool own_thread);

        bool invoke(const Task::Ptr& task);
        bool invoke(const Task::Ptr& task, uint16_t delay);
        bool isCurrentThread();

        TaskQueue::Ptr getTaskQueue() const;

    protected:
        virtual void onDestory();

    private:
        void onDestroyImpl(WPtr wThis);

    private:
        std::unique_ptr<TaskObjectImpl> impl_;
        SpinLock spink_lock_;
        
    };
}

#endif // !src_task_object_h_

