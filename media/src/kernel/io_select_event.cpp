#include "src/kernel/io_select_event.h"

#ifdef WIN32

#include <winsock2.h>

#endif

namespace LJMP {
    IOSelectEvent::IOSelectEvent(const TaskQueue::Ptr& task_queue, bool own_thread) noexcept
        : IOEventStd(task_queue, own_thread) {
    }

    IOSelectEvent::~IOSelectEvent() {
    }

    void IOSelectEvent::onInitialize() {
    }

    void IOSelectEvent::onUninitialize() {
    }

    int IOSelectEvent::onProcess() {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        const IOEventStd::ListChannels& reader_channel = getReadChannels();
        if (!reader_channel.empty()) {
            for (const auto& channel : reader_channel) {
                FD_SET(channel->getFD(), &read_fds);
            }
        }

        fd_set writer_fds;
        FD_ZERO(&writer_fds);
        const IOEventStd::ListChannels& write_channel = getReadChannels();
        if (!write_channel.empty()) {
            for (const auto& channel : write_channel) {
                FD_SET(channel->getFD(), &writer_fds);
            }
        }

        size_t count = getCount();

        TIMEVAL tv = { 1, 10 };
        int ret = ::select(static_cast<int>(count), &read_fds, &writer_fds, nullptr, &tv);
        if (ret == SOCKET_ERROR) {
            return -2;
        }
        else if (0 == ret) {
            return -1;
        }

        for (auto& item : reader_channel) {
            if (FD_ISSET(item->getFD(), &read_fds)) {
                item->readEnable();
            }
        }

        for (auto& item : write_channel) {
            if (FD_ISSET(item->getFD(), &writer_fds)) {
                item->writeEnable();
            }
        }

        return 0;
    }
}
