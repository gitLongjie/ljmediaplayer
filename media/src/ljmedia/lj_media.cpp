#include "ljmedia/lj_media.h"

#include <memory>
#include <mutex>

#include "src/media.h"
#include "src/core/log.h"

std::mutex g_mutex;
LogPtr g_log;
std::shared_ptr<LJMP::Media> g_media;

int initialize(errorCallback callback) {
    if (!g_log) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_log) {
            g_log = LJMP::Log::create();
        }
    }
   
    LOGI("----sdk initialize----");

    if (!g_media) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_media) {
            g_media = std::make_shared<LJMP::Media>();
        }
    }
 
    if (!g_media->initialize(callback)) {
        return -1;
    }
    return 0;
}

void uninitialzie() {
    if (!g_media) {
        return;
    }

    g_media->uninitialize();
    g_media.reset();

    LOGI("----sdk uninitialzie----");
    g_log.reset();
}

int openUrl(const char* szUrl) {
    if (!g_media) {
        return - 1;
    }

    if (!g_media->openUrl(szUrl)) {
        return -2;
    }

    return 0;
}
