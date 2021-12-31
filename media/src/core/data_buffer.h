#ifndef src_data_buffer_h_
#define src_data_buffer_h_

#include <memory>
#include <queue>

#include "src/lj_defined.h"

namespace LJMP {
    class DataBuffer {
        disable_copy(DataBuffer)

    public:
        using Ptr = std::shared_ptr<DataBuffer>;
        using Queue = std::queue<Ptr>;

        static Ptr create(unsigned int size);
        static Ptr create(const char* data, unsigned int size);

    public:
        virtual ~DataBuffer();

        unsigned int getSize() const { return size_ - offset_; }
        const char* getData() const { return data_bffer_; }
        char* getData() { return data_bffer_; }

        void setOffset(unsigned int offset);
        unsigned int getOffset() const { return offset_; }

        int getBaseSize() const { return size_; }
        const char* getBaseData() const { return data_; }
        char* getBaseData() { return data_; }

        void setTimeStamp(long long time_stamp) { time_stamp_ = time_stamp; }
        long long timeStamp() const { return time_stamp_; }
        void setTimeStampEx(long long time_stam_ex) { time_stamp_ex_ = time_stam_ex; }
        long long timeStampEx() const { return time_stamp_ex_; }


    protected:
        explicit DataBuffer(unsigned int size);
        DataBuffer(const char* data, unsigned int size);
        
    private:
        char* data_ = nullptr;
        char* data_bffer_ = nullptr;
        unsigned int size_ = 0;

        unsigned int offset_ = 0;

        long long time_stamp_ = 0;
        long long time_stamp_ex_ = 0;
    };
}

#endif // !src_data_buffer_h_
