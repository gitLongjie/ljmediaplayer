#ifndef src_network_lj_network_define_h_
#define src_network_lj_network_define_h_

#ifdef WIN32

#include <WinSock2.h>

#endif // WIN32

namespace LJMP {
    namespace Network {
#ifdef WIN32

//#include <WinSock2.h>

        using socket_t = SOCKET;

#define getSockError()	WSAGetLastError()

#endif // WIN32
    }
}

#endif // !src_network_lj_network_define_h_
