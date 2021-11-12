#include "src/input/file_input.h"

namespace LJMP {
    namespace Input{
        FileInput::FileInput() : MediaSource() {}

        FileInput::~FileInput() {}

        bool FileInput::load(const std::string& url) {return false;}

        const char* FileInput::protocol() {
            return "file//:";
        }

    } // namespace Input
} // namespace LJMP 
