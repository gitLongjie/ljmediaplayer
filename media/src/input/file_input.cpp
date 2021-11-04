#include "src/input/file_input.h"

namespace LJMP {
    namespace Input{
        FileInput::FileInput() : MediaSource() {}

        FileInput::~FileInput() {}

        bool FileInput::load(const char *szPath) {return false;}
    } // namespace Input
} // namespace LJMP 
