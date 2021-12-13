#include "src/task_queue_object.h"

#include "src/log.h"

namespace LJMP {
    class TaskQueueObjectImpl {
        disable_copy(TaskQueueObjectImpl)

    public:
        static std::unique_ptr<TaskQueueObjectImpl> create(const TaskQueue::Ptr& task_queue, bool own_thread);

    public:
        virtual ~TaskQueueObjectImpl() = default;

        virtual bool invoke(const Task::Ptr& task) = 0;
        virtual bool invoke(const Task::Ptr& task, uint16_t delay) = 0;
        virtual bool isCurrentThread() const = 0;
        virtual TaskQueue::Ptr getTaskQueue() const = 0;

    protected:
        TaskQueueObjectImpl() = default;
    };

    class TaskQueueWPtrImpl : public TaskQueueObjectImpl {
        disable_copy(TaskQueueWPtrImpl)

    public:
        static std::unique_ptr<TaskQueueObjectImpl> create(const TaskQueue::Ptr& task_queue) {
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


    class TaskQueuePtrImpl : public TaskQueueObjectImpl {
        disable_copy(TaskQueuePtrImpl)

    public:
        static std::unique_ptr<TaskQueueObjectImpl> create(const TaskQueue::Ptr& task_queue) {
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

    std::unique_ptr<TaskQueueObjectImpl> TaskQueueObjectImpl::create(
        const TaskQueue::Ptr& task_queue, bool own_thread) {
        if (!own_thread) {
            return TaskQueueWPtrImpl::create(task_queue);
        }
        else {
            return TaskQueuePtrImpl::create(task_queue);
        }
    }

    TaskQueueObject::TaskQueueObject(const TaskQueue::Ptr& task_queue, bool own_thread)
        : impl_(TaskQueueObjectImpl::create(task_queue, own_thread)) {
        LOG_CREATER;
    }

    TaskQueueObject::~TaskQueueObject() {
        LOG_DESTRUCT;
    }

    void TaskQueueObject::destory() {
        LOG_ENTER;

        spink_lock_.lock();
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&TaskQueueObject::onDestroyImpl, this, wThis));
        invoke(task);
    }

    bool TaskQueueObject::invoke(const Task::Ptr& task) {
        return impl_->invoke(task);
    }

    bool TaskQueueObject::invoke(const Task::Ptr& task, uint16_t delay) {
        return impl_->invoke(task, delay);
    }

    bool TaskQueueObject::isCurrentThread() {
        return impl_->isCurrentThread();
    }

    TaskQueue::Ptr TaskQueueObject::getTaskQueue() const {
        return impl_->getTaskQueue();
    }

    void TaskQueueObject::onDestory() {

    }

    void TaskQueueObject::onDestroyImpl(WPtr wThis) {
        LOG_ENTER;

        Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destructed");
            return;
        }

        onDestory();

        spink_lock_.unlock();

    }

}
