#include "src/network/win/network_manager_win.h"

#include <WinSock2.h>
#include <windows.h>

#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Network {

        NetworkManagerPtr NetworkManagerWin::create(TaskQueuePtr task_queue) {
            struct Creator : public NetworkManagerWin {
                Creator(TaskQueuePtr task_queue) : NetworkManagerWin(task_queue) {}
                ~Creator() override = default;
            };
            return std::make_shared<Creator>(task_queue);
        }

        NetworkManagerWin::NetworkManagerWin(TaskQueuePtr task_queue)
            : NetworkManagerStd(task_queue) {
            LOGI("actor {}", (long long)this);
        }

        NetworkManagerWin::~NetworkManagerWin() {
            LOGI("dctor {}", (long long)this);
        }

        void NetworkManagerWin::doInitialize(NetworkManagerWPtr wThis) {
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

        void NetworkManagerWin::doUninitialize(NetworkManagerWPtr wThis) {
            LOG_ENTER;

            NetworkManagerPtr self = wThis.lock();
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