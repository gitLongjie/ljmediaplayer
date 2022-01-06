#include "src/kernel/channel_std.h"

#include "src/core/log.h"
#include "src/kernel/network_manager.h"
#include "src/media.h"

namespace LJMP {
	ChannelStd::ChannelStd(const IReadWriter::Ptr& read_writer, const IReadWriterCallback::Ptr& callback)
		: ObjectPtr()
		, read_writer_(read_writer)
		, read_write_callback_(callback){
		LOG_CREATER;
	}

	ChannelStd::~ChannelStd() {
		LOG_DESTRUCT;
	}

	FD ChannelStd::getFD() const {
		if (!read_writer_) {
			return -1;
		}
		return read_writer_->getFD();;
	}

	void ChannelStd::updateEvent(unsigned int io_event) {
		LOG_ENTER;

		IChannel::Ptr self = std::dynamic_pointer_cast<IChannel>(shared_from_this());
		Media::getInstance()->getNetworkManager()->updateChannel(self, io_event);
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
