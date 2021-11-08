//  Copyright 2020 LJ
#ifndef src_log_h_
#define src_log_h_

#include <functional>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>

namespace LJMP {

using LogWriteCallback = std::function<void(int, const std::string&)>;

class Log {
public:
    enum LogLevel {
        LOG_DEBUG_MESSAGE = 0,
        LOG_INFO_MESSAGE,
        LOG_WARNING_MESSAGE,
        LOG_ERROR_MESSAGE,
        LOG_CRITICAL_MESSAGE,
    };
    static std::shared_ptr<Log> create();
    static Log* GetInstance();
    
public:
    Log();
    ~Log();
    
    void SetLogWriteCallback(LogWriteCallback callback);
    template <typename... Args>
    void Write(LogLevel logLevel, const char* fromat,
             const Args&... args) {
    spdlog::memory_buf_t buf;
    fmt::format_to(buf, fromat, args...);

    Write(logLevel, buf.data(), buf.size());
    }
    void Write(LogLevel logLevel, const std::string& log);

protected:
    void Write(LogLevel logLevel, const char* log, size_t len);

private:
    LogWriteCallback m_logWirateCallback;
    std::shared_ptr<spdlog::logger> m_logger;
};
}  // namespace LJMP

#define LOGD(format, ...)                                               \
  SMUnity::Core::Log::GetInstance()->Write(SMUnity::Core::Log::LOG_DEBUG_MESSAGE, "FILE:{}, FUNCTION:{},"##format, \
                            __FILE__, __FUNCTION__, ##__VA_ARGS__)
#define LOGE(format, ...)                                               \
  SMUnity::Core::Log::GetInstance()->Write(SMUnity::Core::Log::LOG_ERROR_MESSAGE, "FILE:{}, FUNCTION:{},"##format, \
                            __FILE__, __FUNCTION__, ##__VA_ARGS__)
#define LOGW(format, ...)                                               \
  SMUnity::Core::Log::GetInstance()->Write(SMUnity::Core::Log::LOG_WARNING_MESSAGE, "FILE:{}, FUNCTION:{},"##format, \
                            __FILE__, __FUNCTION__, ##__VA_ARGS__)
#define LOGI(format, ...)                                               \
  SMUnity::Core::Log::GetInstance()->Write(SMUnity::Core::Log::LOG_INFO_MESSAGE, "FILE:{}, FUNCTION:{},"##format, \
                            __FILE__, __FUNCTION__, ##__VA_ARGS__)
#define LOGC(format, ...)                                             \
  SMUnity::Core::Log::GetInstance()->Write(SMUnity::Core::Log::LOG_CRITICAL_MESSAGE, "FILE:{}, FUNCTION:{},"##format, \
                            __FILE__, __FUNCTION__, ##__VA_ARGS__)

#define LOG_ENTER LOGI("enter")
#define LOG_LIVE LOGI("live")

#endif // !src_log_h_
