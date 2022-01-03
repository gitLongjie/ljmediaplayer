#ifndef src_network_socket_factory_h_
#define src_network_socket_factory_h_

#include "src/lj_defined.h"

#include "src/network/socket.h"

namespace LJMP {
	namespace Network {
		class SocketFactory {
			disable_copy(SocketFactory);

		public:
			SocketFactory() = default;
			~SocketFactory() = default;

			Socket::Ptr createTcp() noexcept;
		};
	}
}

#endif // ! src_network_socket_factory_h_
