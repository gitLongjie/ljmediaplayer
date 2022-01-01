#ifndef src_core_task_queue_h_
#define src_core_task_queue_h_

#include <thread>
#include <atomic>
#include <queue>
#include <map>
#include <functional>
#include <memory>
#include <mutex>

void setThreadName(const char* szName);
void setStdThreadName(std::thread* thread, const char* szName);

class Task {
public:
    using Ptr = std::shared_ptr<Task>;

public:
    virtual ~Task(void) = default;
    virtual void execute(void) = 0;
    //    virtual bool operator>(std::shared_ptr<Task>& other) = 0;
};

template <class F, class C = std::function<void(void)>>
class TaskImpl : public Task {
public:
    TaskImpl(F function, C callback)
        : m_pFunction(function)
        , m_pCallback(callback) {
    }
    ~TaskImpl() override = default;

    TaskImpl(const TaskImpl&) = delete;
    TaskImpl& operator=(const TaskImpl&) = delete;

    void execute(void) override {
        m_pFunction();
        if (nullptr != m_pCallback) {
            m_pCallback();
        }
    }

private:
    F m_pFunction;
    C m_pCallback;
};

template <class F, class C = std::function<void(void)>>
Task::Ptr createTask(F function, C callback = nullptr) {
    return std::make_shared<TaskImpl<F, C>>(function, callback);
}

template <class Fx, typename... Args>
Task::Ptr createTaskNoCallback(Fx&& function, Args&&... args) {
    auto func = std::bind(std::forward<Fx>(function), std::forward<Args>(args)...);
    return std::make_shared<TaskImpl<decltype(func)>>(func, nullptr);
}

class TaskQueue {
public:
    using Ptr = std::shared_ptr<TaskQueue>;
    using WPtr = std::weak_ptr<TaskQueue>;
    static Ptr create(const char* name);

public:
    TaskQueue(const char* szName);
    ~TaskQueue();

    void push(Task::Ptr task);
    void push(Task::Ptr task, uint16_t delay);

    bool isCurrentThread() const;

    void stop();

private:
    void                  dispatch();
    Task::Ptr popNormalTask();
    Task::Ptr popDelayTask();

private:
    std::atomic_bool             m_terminated;
    std::unique_ptr<std::thread> m_thread;

    using NormalListTask = std::queue<Task::Ptr>;
    using DelayListTask =
        std::map<std::chrono::high_resolution_clock::time_point, Task::Ptr>;

    std::mutex              m_mutex;
    std::condition_variable m_condition_variable;
    NormalListTask          m_normalListTask;
    DelayListTask           m_delayListTask;

    std::thread::id m_currentId = std::this_thread::get_id();
};

#endif // src_core_task_queue_h_
