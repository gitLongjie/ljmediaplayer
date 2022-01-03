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

	void ChannelStd::readEnable() {
		if (!read_writer_) {
			LOGE("read writer is nullptr");
			return;
		}

		if (!read_write_callback_) {
			LOGD("read write callback is nullptr");
			return;
		}

		read_write_callback_->onRead(read_writer_);
	}

	void ChannelStd::writeEnable() {
		if (!read_writer_) {
			LOGE("read writer is nullptr");
			return;
		}

		if (!read_write_callback_) {
			LOGD("read write callback is nullptr");
			return;
		}

		read_write_callback_->onWrite(read_writer_);
	}
	
}
