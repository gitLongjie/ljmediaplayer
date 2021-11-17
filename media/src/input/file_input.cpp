#include "src/input/file_input.h"

namespace LJMP {
    namespace Input{
        FileInput::FileInput() : MediaSource(nullptr) {}

        FileInput::~FileInput() {}

        const char* FileInput::protocol() {
            return "file//:";
        }

        bool FileInput::doOpen(const std::string& url) {
            return false;
        }

        void FileInput::doClose() {

        }

    } // namespace Input
} // namespace LJMP 
