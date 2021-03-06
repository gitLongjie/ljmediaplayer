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

		FD getFD() const override;
		void setReadWriteCallback(const IReadWriterCallback::Ptr& callback) override {
			read_write_callback_ = callback;
		}
		void updateEvent(unsigned int io_event) override;
		void readEnable() override;
		void writeEnable() override;

	protected:
		ChannelStd( const IReadWriter::Ptr& read_writer, const IReadWriterCallback::Ptr& callback);

	private:
		IReadWriter::Ptr read_writer_;
		IReadWriterCallback::Ptr read_write_callback_;
	};
}
#endif // !src_kernel_channel_std_h_



