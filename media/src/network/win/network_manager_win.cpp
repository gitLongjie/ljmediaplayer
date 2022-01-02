#include "src/network/win/network_manager_win.h"

#include <WinSock2.h>
#include <windows.h>

#include "src/core/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Network {

        INetworkManager::Ptr NetworkManagerWin::create(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue) {
            return createPtr<NetworkManagerWin>(io_event, task_queue);
        }

        NetworkManagerWin::NetworkManagerWin(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue)
            : NetworkManagerStd(io_event, task_queue) {
            LOGI("actor {}", (long long)this);
        }

        NetworkManagerWin::~NetworkManagerWin() {
            LOGI("dctor {}", (long long)this);
        }

        void NetworkManagerWin::doInitialize(ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            LOGI("init network");
            WSAData wsaData;
            if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
                LOGE("initialize net work failed err={}", WSAGetLastError());
                Media::getInstance()->errorCallbak(error_code_netork_initialize_failed, "net initialize failed");
                return;
            }

            NetworkManagerStd::doInitialize(wThis);
        }

        void NetworkManagerWin::doUninitialize(ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGW("this object is destruct");
                return;
            }

            LOGI("clear network");
            WSACleanup();
            NetworkManagerStd::doUninitialize(wThis);
        }

    }
}