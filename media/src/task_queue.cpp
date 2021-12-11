#include "src/task_queue.h"

#include <chrono>

#ifdef _WIN32

#include <windows.h>
#include <excpt.h>

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO {
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)


void SetThreadName(uint32_t dwThreadID, const char* threadName) {

    // DWORD dwThreadID = ::GetThreadId( static_cast<HANDLE>( t.native_handle() ) );

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
    }
}
void SetWindowsThreadName(const char* threadName) {
    ::SetThreadName(::GetCurrentThreadId(), threadName);
}

void SetWindowsThreadName(std::thread* thread, const char* threadName) {
    DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
    ::SetThreadName(threadId, threadName);
}
#endif

void setThreadName(const char* szName) {
    SetWindowsThreadName(szName);
}


void setStdThreadName(std::thread* thread, const char* szName) {
    SetWindowsThreadName(thread, szName);
}

TaskQueue::Ptr TaskQueue::create(const char* name) {
    return std::make_shared<TaskQueue>(name);
}

TaskQueue::TaskQueue(const char* szName)
    : m_terminated(false) {
    m_thread.reset(new std::thread([this]() { dispatch(); }));
    m_currentId = m_thread->get_id();
    setStdThreadName(m_thread.get(), szName);
}

TaskQueue::~TaskQueue() {
   // stop();
}

void TaskQueue::push(Task::Ptr task) {
    if (m_terminated) {
        return;
    }
    std::unique_lock<std::mutex> lock(m_mutex);
    m_normalListTask.push(std::move(task));
    m_condition_variable.notify_one();
}

void TaskQueue::push(Task::Ptr task, uint16_t delay) {
    if (m_terminated) {
        return;
    }

    if (0 == delay) {
        push(task);
        return;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto run_time = now + std::chrono::milliseconds(delay);
    std::unique_lock<std::mutex> lock(m_mutex);
    m_delayListTask.insert(std::make_pair(run_time, std::move(task)));
    m_condition_variable.notify_one();
}

bool TaskQueue::isCurrentThread() const {
    return std::this_thread::get_id() == m_currentId;
}

void TaskQueue::stop() {
    m_terminated.store(true);
    m_condition_variable.notify_all();
    if (m_thread->joinable()) {
        m_thread->join();
    }
}

void TaskQueue::dispatch() {
    while (!m_terminated.load()) {
        auto normalTask = popNormalTask();
        if (normalTask) {
            normalTask->execute();
        }

        auto delayTask = popDelayTask();
        if (delayTask) {
            delayTask->execute();
        }
        // auto del
    }
}

Task::Ptr TaskQueue::popNormalTask() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_normalListTask.empty()) {
        return nullptr;
    }

    auto task = m_normalListTask.front();
    m_normalListTask.pop();
    return task;
}

Task::Ptr TaskQueue::popDelayTask() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (!m_delayListTask.empty()) {
        auto begin = m_delayListTask.begin();
        auto now = std::chrono::high_resolution_clock::now();
        if (now > begin->first) {
            auto task = begin->second;
            m_delayListTask.erase(begin);
            return task;
        }
        else {
            m_condition_variable.wait_until(lock, begin->first, [=]() {
                return !m_normalListTask.empty() || m_terminated.load();
            });
        }

    }
    else {
        m_condition_variable.wait(lock, [=]() {
            return !m_normalListTask.empty() || !m_delayListTask.empty() ||
                m_terminated.load();
        });
    }

    if (m_terminated.load()) {
        return nullptr;
    }
    else if (m_normalListTask.empty()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto begin = m_delayListTask.begin();
        if (now > begin->first) {
            auto task = begin->second;
            m_delayListTask.erase(begin);
            return task;
        }
    }
    return nullptr;
}
