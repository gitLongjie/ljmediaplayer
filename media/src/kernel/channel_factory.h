#ifndef src_kernel_channel_factory_h_
#define src_kernel_channel_factory_h_

#include "src/lj_defined.h"

#include "src/kernel/read_writer.h"
#include "src/kernel/read_writer_callback.h"
#include "src/kernel/channel.h"

namespace LJMP {
	class ChannelFactory {
		disable_copy(ChannelFactory);

	public:
		ChannelFactory() = default;
		~ChannelFactory() = default;

		IChannel::Ptr create(const IReadWriter::Ptr& read_writer, const IReadWriterCallback::Ptr& callback);
	};
}

#endif // !src_kernel_channel_factory_h_


