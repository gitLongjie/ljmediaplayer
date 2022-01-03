#include "src/network/socket_factory.h"

#include "src/network/tcp_socket.h"

namespace LJMP {
	namespace Network {
		Socket::Ptr SocketFactory::createTcp() noexcept
		{
			return TcpSocket::create();
		}
	}
}
