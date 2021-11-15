#include "src/network/win/network_manager_win.h"

#include <WinSock2.h>
#include <windows.h>

#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Network {

        NetworkManagerPtr NetworkManagerWin::create(TaskQueuePtr taskQueue) {
            struct Creator : public NetworkManagerWin {
                Creator(TaskQueuePtr taskQueue) : NetworkManagerWin(taskQueue) {}
                ~Creator() override = default;
            };
            return std::make_shared<Creator>(taskQueue);
        }

        NetworkManagerWin::NetworkManagerWin(TaskQueuePtr taskQueue)
            : NetworkManagerStd(taskQueue) {
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