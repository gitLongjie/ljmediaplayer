#ifndef src_media_thread_pool_h_
#define src_media_thread_pool_h_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <functional>
#include <vector>

namespace LJMP {
    class ThreadPool {
    public:
        static std::shared_ptr<ThreadPool> create(size_t size) {
            struct Creator : public ThreadPool {
                Creator(size_t size) : ThreadPool(size) {}
                ~Creator()  override = default;
            };

            return std::make_shared<Creator>(size);
        }
    public:
        virtual ~ThreadPool() {
            stop();
        }

        void stop() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                if (stop_) { return; }
                stop_ = true;
            }
            
            condition_.notify_all();
            for (auto& work : works_) {
                if (work.joinable()) {
                    work.join();
                }
            }
        }

        template <class F, typename... Args>
        auto pushTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
            using result_type = decltype(f(args...));

            {
                std::unique_lock<std::mutex> lock;
                if (stop_) {
                    throw std::runtime_error("this thread pool is stopped");
                }
            }

            auto task = std::make_shared<std::packaged_task<result_type()>>(std::bind(std::forward<F>(f),
                std::forward<Args>(args)...));
            auto res = task->get_future();
            {
                std::unique_lock<std::mutex> lock;
                if (stop_) {
                    throw std::runtime_error("this thread pool is stopped");
                }
                tasks_.emplace([task]() {(*task)(); });
            }

            condition_.notify_one();
            return res;
        }

    protected:
        ThreadPool(size_t size) {
            stop_ = false;
            for (size_t index = 0; index < size; ++index) {
                works_.emplace_back(std::thread([this]() {
                    while (true) {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(queue_mutex_);
                            condition_.wait(lock, [this]()->bool {return !tasks_.empty() || stop_; });
                            if (stop_) {
                                break;
                            }
                            else if (tasks_.empty()) {
                                continue;
                            }

                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }

                        task();
                    }
                    }));
            }
        }

    private:
        std::vector<std::thread> works_;
        std::queue< std::function<void()> > tasks_;

        std::mutex queue_mutex_;
        std::condition_variable condition_;
        bool stop_ = true;
    };

    using ThreadPoolPtr = std::shared_ptr<ThreadPool>;
}

#endif // !src_media_thread_pool_h_
