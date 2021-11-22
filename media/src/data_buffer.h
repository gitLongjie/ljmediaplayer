#ifndef src_data_buffer_h_
#define src_data_buffer_h_

#include <memory>
#include "src/lj_defined.h"

namespace LJMP {
    class DataBuffer {
        disable_copy(DataBuffer)

    public:
        using Ptr = std::shared_ptr<DataBuffer>;

        static Ptr create(unsigned int size);
        static Ptr create(const char* data, unsigned int size);

    public:
        virtual ~DataBuffer();

        unsigned int getSize() const { return size_; }
        const char* getData() const { return data_; }
        char* getData() { return data_; }

        void setOffset(unsigned int offset) { offset_ = offset; }
        unsigned int getOffset() const { return offset_; }
        const char* getOffsetData() const { return data_ + offset_; }
        char* getOffsetData() { return data_ + offset_; }


    protected:
        explicit DataBuffer(unsigned int size);
        DataBuffer(const char* data, unsigned int size);
        
    private:
        char* data_ = nullptr;
        unsigned int size_ = 0;

        unsigned int offset_ = 0;
    };
}

#endif // !src_data_buffer_h_
