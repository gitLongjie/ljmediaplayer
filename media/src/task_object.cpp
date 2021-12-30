#include "src/task_object.h"

#include "src/log.h"

namespace LJMP {
    class TaskObjectImpl {
        disable_copy(TaskObjectImpl)

    public:
        static std::unique_ptr<TaskObjectImpl> create(const TaskQueue::Ptr& task_queue, bool own_thread);

    public:
        virtual ~TaskObjectImpl() = default;

        virtual bool invoke(const Task::Ptr& task) = 0;
        virtual bool invoke(const Task::Ptr& task, uint16_t delay) = 0;
        virtual bool isCurrentThread() const = 0;
        virtual TaskQueue::Ptr getTaskQueue() const = 0;

    protected:
        TaskObjectImpl() = default;
    };

    class TaskQueueWPtrImpl : public TaskObjectImpl {
        disable_copy(TaskQueueWPtrImpl)

    public:
        static std::unique_ptr<TaskObjectImpl> create(const TaskQueue::Ptr& task_queue) {
            struct Creator : public TaskQueueWPtrImpl {
                explicit Creator(const TaskQueue::Ptr& task_queue) : TaskQueueWPtrImpl(task_queue) {}
                ~Creator() override {}
            };
            return std::make_unique<Creator>(task_queue);
        }

    public:
        ~TaskQueueWPtrImpl() override = default;

        bool invoke(const Task::Ptr& task) override {
            TaskQueue::Ptr task_queue(task_queue_.lock());
            if (!task_queue) {
                LOGE("this task queue is destruct");
                return false;
            }

            if (task_queue->isCurrentThread()) {
                task->execute();
                return true;
            }

            task_queue->push(task);
            return true;
        }

        bool invoke(const Task::Ptr& task, uint16_t delay) override {
            TaskQueue::Ptr task_queue(task_queue_.lock());
            if (!task_queue) {
                LOGE("this task queue is destruct");
                return false;
            }

            task_queue->push(task, delay);
            return true;
        }

        bool isCurrentThread() const override {
            TaskQueue::Ptr task_queue(task_queue_.lock());
            if (!task_queue) {
                LOGE("this task queue is destruct");
                return true;
            }

            return task_queue->isCurrentThread();
        }

        TaskQueue::Ptr getTaskQueue() const override {
            return task_queue_.lock();
        }

    protected:
        explicit TaskQueueWPtrImpl(const TaskQueue::Ptr& task_queue) :task_queue_(task_queue) {}

    private:
        TaskQueue::WPtr task_queue_;
    };


    class TaskQueuePtrImpl : public TaskObjectImpl {
        disable_copy(TaskQueuePtrImpl)

    public:
        static std::unique_ptr<TaskObjectImpl> create(const TaskQueue::Ptr& task_queue) {
            struct Creator : public TaskQueuePtrImpl {
                explicit Creator(const TaskQueue::Ptr& task_queue) : TaskQueuePtrImpl(task_queue) {}
                ~Creator() override { }
            };
            return std::make_unique<Creator>(task_queue);
        }
    public:
        ~TaskQueuePtrImpl() override = default;

        bool invoke(const Task::Ptr& task) override {
            if (!task_queue_) {
                LOGE("this task queue is destruct");
                return false;
            }

            if (task_queue_->isCurrentThread()) {
                task->execute();
                return true;
            }

            task_queue_->push(task);
            return true;
        }

        bool invoke(const Task::Ptr& task, uint16_t delay) override {
            if (!task_queue_) {
                LOGE("this task queue is destruct");
                return false;
            }

            task_queue_->push(task, delay);
            return true;
        }

        bool isCurrentThread() const override {
            if (!task_queue_) {
                LOGE("this task queue is destruct");
                return true;
            }

            return task_queue_->isCurrentThread();
        }

        TaskQueue::Ptr getTaskQueue() const override {
            return task_queue_;
        }

    protected:
        explicit TaskQueuePtrImpl(const TaskQueue::Ptr& task_queue) :task_queue_(task_queue) {}

    private:
        TaskQueue::Ptr task_queue_;
    };

    std::unique_ptr<TaskObjectImpl> TaskObjectImpl::create(
        const TaskQueue::Ptr& task_queue, bool own_thread) {
        if (!own_thread) {
            return TaskQueueWPtrImpl::create(task_queue);
        }
        else {
            return TaskQueuePtrImpl::create(task_queue);
        }
    }

    TaskObject::TaskObject(const TaskQueue::Ptr& task_queue, bool own_thread)
        : impl_(TaskObjectImpl::create(task_queue, own_thread)) {
        LOG_CREATER;
    }

    TaskObject::~TaskObject() {
        LOG_DESTRUCT;
    }

    void TaskObject::destory() {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&TaskObject::onDestroyImpl, this, wThis));
        invoke(task);
        spink_lock_.lock();
    }

    bool TaskObject::invoke(const Task::Ptr& task) {
        return impl_->invoke(task);
    }

    bool TaskObject::invoke(const Task::Ptr& task, uint16_t delay) {
        return impl_->invoke(task, delay);
    }

    bool TaskObject::isCurrentThread() {
        return impl_->isCurrentThread();
    }

    TaskQueue::Ptr TaskObject::getTaskQueue() const {
        return impl_->getTaskQueue();
    }

    void TaskObject::onDestory() {

    }

    void TaskObject::onDestroyImpl(WPtr wThis) {
        LOG_ENTER;

        ObjectPtr::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destructed");
            spink_lock_.unlock();
            return;
        }

        onDestory();

        spink_lock_.unlock();

    }

}
