#include "ljmedia/lj_media.h"

#include <memory>

#include "src/media.h"

std::shared_ptr<LJMP::Media> g_media;

int initialize() {
    g_media = std::make_shared<LJMP::Media>();
    if (!g_media->initialize()) {
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
