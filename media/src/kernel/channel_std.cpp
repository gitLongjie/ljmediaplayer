#include "src/kernel/channel_std.h"

#include "src/core/log.h"

namespace LJMP {
	ChannelStd::ChannelStd(const IIOEvent::Ptr& io_event, const IReadWriter::Ptr& read_writer)
		: ObjectPtr()
		, io_event_(io_event)
		, read_writer_(read_writer) {
		LOG_CREATER;
	}

	ChannelStd::~ChannelStd() {
		LOG_DESTRUCT;
	}
	
}
