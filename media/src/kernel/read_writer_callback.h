#ifndef src_kernel_read_writer_callback_h_
#define src_kernel_read_writer_callback_h_

#include "src/kernel/read_writer.h"

namespace LJMP {
	class IReaderCallback {
	public:
		using Ptr = std::shared_ptr<IReaderCallback>;
		using WPtr = std::weak_ptr<IReaderCallback>;

	public:
		virtual ~IReaderCallback() = default;
		virtual bool onRead(const IReader::Ptr& reader) = 0;
	};

	class IWriterCallback {
	public:
		using Ptr = std::shared_ptr<IWriterCallback>;
		using WPtr = std::weak_ptr<IWriterCallback>;

	public:
		virtual ~IWriterCallback() = default;
		virtual bool onWrite(const IWriter::Ptr& reader) = 0;
	};

	class IReadWriterCallback : public IReaderCallback, public IWriterCallback {
	public:
		using Ptr = std::shared_ptr<IReadWriterCallback>;
		using WPtr = std::weak_ptr<IReadWriterCallback>;

	public:
		~IReadWriterCallback() override = default;
	};
}

#endif // !src_kernel_read_writer_callback_h_

