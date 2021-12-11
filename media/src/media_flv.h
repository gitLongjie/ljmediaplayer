#ifndef src_media_flv_h_
#define src_media_flv_h_

#include "src/utils.h"

namespace LJMP {
#pragma pack(1)
	struct FLVHeader {
		unsigned char sig[3];
		unsigned char version;
		unsigned char flag;
		unsigned int offset;

		bool isContainsAudio() const {
			return flag & 0x04;
		}

		bool isContainsVideo() const {
			return flag & 0x01;
		}

		bool isContainsMedia() const {
			return flag & 0x05;
		}
	};

	enum class FLVType { SCRIPTE = 0x12, AUDIO = 0x08, VIDEO = 0x09};

	struct FLVTagHeader {
		unsigned int back_pointer;
		unsigned char type;
		unsigned char tag_size[3];
		unsigned char timestamp[3];
		unsigned char timestampex;
		char stream_id[3];

		unsigned int getTagSize() const {
			unsigned int size = 0;
			memcpy(&size, tag_size, sizeof(unsigned char) * 3);
			return Utils::ntohl(size) >> 8;;
		}

        unsigned int getTimestamp() const {
            unsigned int size = 0;
            memcpy(&size, timestamp, sizeof(unsigned char) * 3);
            return Utils::ntohl(size) >> 8;;
        }
	};
#pragma pack()
}

#endif // !src_media_flv_h_

