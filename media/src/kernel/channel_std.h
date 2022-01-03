#ifndef src_kernel_channel_std_h_
#define src_kernel_channel_std_h_

#include "src/kernel/channel.h"
#include "src/core/object_sptr.h"
#include "src/kernel/io_event.h"
#include "src/kernel/read_writer.h"
#include "src/kernel/read_writer_callback.h"

namespace LJMP{
	class ChannelStd : public ObjectPtr, public IChannel {
		disable_copy(ChannelStd);

	public:
		~ChannelStd() override;

		void setReadWriteCallback(const std::shared_ptr<IReadWriterCallback>& callback) override {
			read_write_callback_ = callback;
		}
		void readEnable() override;
		void writeEnable() override;

	protected:
		ChannelStd(const IIOEvent::Ptr& io_event, const IReadWriter::Ptr& read_writer);

	private:
		IIOEvent::WPtr io_event_;
		IReadWriter::Ptr read_writer_;
		IReadWriterCallback::Ptr read_write_callback_;
	};
}
#endif // !src_kernel_channel_std_h_



