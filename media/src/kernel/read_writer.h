#ifndef src_kernel_read_writer_h_
#define src_kernel_read_writer_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class IReader {
    public:
        using Ptr = std::shared_ptr<IReader>;
        using WPtr = std::weak_ptr<IReader>;
    public:
        virtual ~IReader() = default;
        virtual int read(unsigned char* buffer, unsigned int size) = 0;
    };

    class IWriter {
    public:
        using Ptr = std::shared_ptr<IWriter>;
        using WPtr = std::weak_ptr<IWriter>;

    public:
        virtual ~IWriter() = default;
        virtual int write(const unsigned char* buffer, unsigned int size) = 0;
    };

    class IReadWriter : public IReader, public IWriter {
    public:
        using Ptr = std::shared_ptr<IReadWriter>;
        using WPtr = std::weak_ptr<IReadWriter>;

    public:
        ~IReadWriter() override = default;

        virtual FD getFD() const = 0;
    };
}

#endif // !src_kernel_read_writer_h_

