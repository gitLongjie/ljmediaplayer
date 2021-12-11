#include "src/data_buffer.h"

namespace LJMP {

    DataBuffer::Ptr DataBuffer::create(unsigned int size) {
        return createPtr<DataBuffer>(size);
    }

    DataBuffer::Ptr DataBuffer::create(const char* data, unsigned int size) {
        struct Creator : public DataBuffer {
            explicit Creator(const char* data, unsigned int size) : DataBuffer(data, size) {}
            ~Creator() override = default;
        };

        return std::make_shared<Creator>(data,size);
    }

    DataBuffer::DataBuffer(unsigned int size) 
        : size_(size) {
        if (data_) {
            delete[] data_;
        }

        data_ = new char[size];
        size_ = size;
    }

    DataBuffer::DataBuffer(const char* data, unsigned int size) 
        : size_(size) {
        if (data_) {
            delete[] data_;
        }

        data_ = new char[size];
        size_ = size;
        memcpy(data_, data, size);
    }

    DataBuffer::~DataBuffer() {
        if (nullptr != data_) {
            delete []data_;
        }
    }

}
