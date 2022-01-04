#ifndef src_kernel_read_writer_callback_std_h_
#define src_kernel_read_writer_callback_std_h_

#include "src/lj_defined.h"

#include "src/core/object_sptr.h"
#include "src/kernel/read_writer_callback.h"

namespace LJMP {
	class ReadWriterCallbackStd : public ObjectPtr, public IReadWriterCallback {
		disable_copy(ReadWriterCallbackStd);

	public:
		~ReadWriterCallbackStd() override;

		bool onRead(const IReader::Ptr& reader) override { return false; }
		bool onWrite(const IWriter::Ptr& writer) override { return false; }

	protected:
		ReadWriterCallbackStd();
	};
}

#endif // !src_kernel_read_writer_callback_std_h_


