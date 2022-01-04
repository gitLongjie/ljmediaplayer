#include "src/input/rtmp/rtmp_read_writer_callback.h"

#include "src/core/log.h"

namespace LJMP {
	namespace Input {
		namespace Rtmp {
			
			RtmpReadWriteCallback::RtmpReadWriteCallback()
				: ReadWriterCallbackStd() {
				LOG_CREATER;
			}

			RtmpReadWriteCallback::~RtmpReadWriteCallback() {
				LOG_DESTRUCT;
			}

			bool RtmpReadWriteCallback::onRead(const IReader::Ptr& reader) {
				return false;
			}

			bool RtmpReadWriteCallback::onWrite(const IWriter::Ptr& writer) {
				return false;
			}

		}
	}
}
