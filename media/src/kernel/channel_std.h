#ifndef src_kernel_channel_std_h_
#define src_kernel_channel_std_h_

#include "src/kernel/channel.h"
#include "src/core/object_sptr.h"
#include "src/kernel/io_event.h"
#include "src/kernel/read_writer.h"

namespace LJMP{
	class ChannelStd : public ObjectPtr, public IChannel {
		disable_copy(ChannelStd);

	public:
		~ChannelStd() override;

	protected:
		ChannelStd(const IIOEvent::Ptr& io_event, const IReadWriter::Ptr& read_writer);

	private:
		IIOEvent::WPtr io_event_;
		IReadWriter::Ptr read_writer_;
	};
}
#endif // !src_kernel_channel_std_h_



