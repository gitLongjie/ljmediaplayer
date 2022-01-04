#ifndef src_input_rtmp_rtmp_read_writer_callback_h_
#define src_input_rtmp_rtmp_read_writer_callback_h_

#include "src/lj_defined.h"
#include "src/kernel/read_writer_callback_std.h"

namespace LJMP {
	namespace Input {
		namespace Rtmp {
			class RtmpReadWriteCallback : public ReadWriterCallbackStd {
				disable_copy(RtmpReadWriteCallback)

			public:
				~RtmpReadWriteCallback() override;

				bool onRead(const IReader::Ptr& reader) override;
				bool onWrite(const IWriter::Ptr& writer) override;

			protected:
				RtmpReadWriteCallback();
			};
		}
	}
}
#endif // !src_input_rtmp_rtmp_read_writer_callback_h_

