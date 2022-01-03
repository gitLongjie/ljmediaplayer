#ifndef src_kernel_channel_h_
#define src_kernel_channel_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
	
	class IChannel {
	public:
		using Ptr = std::shared_ptr<IChannel>;
		using WPtr = std::weak_ptr<IChannel>;

	public:
		virtual ~IChannel() = default;
		virtual FD getFD() const = 0;

		virtual void readEnable() = 0;
		virtual void writeEnable() = 0;
	};

}
#endif // !src_kernel_channel_h_

