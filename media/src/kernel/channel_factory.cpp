#include "src/kernel/channel_factory.h"

#include "src/kernel/channel_std.h"

namespace LJMP {
	IChannel::Ptr ChannelFactory::create(const IReadWriter::Ptr& read_writer,
		const IReadWriterCallback::Ptr& callback) {
		return createPtr<ChannelStd>(read_writer, callback);
	}
}
