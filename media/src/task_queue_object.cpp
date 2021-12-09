#include "src/task_queue_object.h"

#include "src/log.h"

namespace LJMP {

    TaskQueueObject::TaskQueueObject(const TaskQueue::Ptr& task_queue)
        : task_queue_(task_queue) {
        LOG_CREATER;
    }

    TaskQueueObject::~TaskQueueObject() {
        LOG_DESTRUCT;
    }

    bool TaskQueueObject::invoke(const Task::Ptr& task) {
        TaskQueue::Ptr task_queue(task_queue_.lock());
        if (!task_queue) {
            LOGE("this task queue is destruct");
            return false;
        }

        task_queue->push(task);
        return true;
    }

    bool TaskQueueObject::invoke(const Task::Ptr& task, uint16_t delay) {

        TaskQueue::Ptr task_queue(task_queue_.lock());
        if (!task_queue) {
            LOGE("this task queue is destruct");
            return false;
        }

        if (task_queue->isCurrentThread()) {
            task->execute();
            return false;
        }

        task_queue->push(task, delay);
        return true;
    }

    bool TaskQueueObject::isCurrentThread() {
        TaskQueue::Ptr task_queue(task_queue_.lock());
        if (!task_queue) {
            LOGE("this task queue is destruct");
            return true;
        }

        return task_queue->isCurrentThread();
    }

}
