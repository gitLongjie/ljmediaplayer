#ifndef src_input_file_input_h_
#define src_input_file_input_h_

#include "src/media_source.h"

namespace LJMP {
    namespace Input{
        class FileInput : public MediaSource {
            disable_copy(FileInput)

        public:
            static const char* protocol();
        public:
            FileInput();
            ~FileInput() override;

        protected:
            bool doOpen(const std::string& url) override;
            void doClose() override;
        };
    } // namespace Input
} // namespace LJMP 

#endif // !src_input_file_input_h_
