#include <iostream>

#ifdef WIN32
#include <conio.h>
#include <windows.h>

#define getKey _kbhit
#endif

#include <ljmedia/lj_media.h>


void error_callback(int code, const char* msg) {
    std::cout << code << " :" << msg << std::endl;
}

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
#pragma pack()
#pragma pack(1)
struct FLVTagHeader {
    unsigned int back_pointer;
    unsigned char type;
    unsigned int tag_size : 24;
    unsigned int timestamp : 24;
    unsigned char timestampex;
    char stream_id[3];
};
#pragma pack()


int main() {
    std::cout << "hello c++" << std::endl;

    const char* s = "1234567890";
    const char* ss = "78";
    const char* ret = strstr(s, ss);
    if (nullptr == ret) {
        return -1.0;
    } 
    if (0 != initialize(error_callback)) {
        return -1;
    }

    std::string str = std::string("a", 1);
    unsigned int n = sizeof(FLVTagHeader);
    // const char* test = "rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp";
    //const char* test = "rtmp://ns8.indexforce.com/home/mystream";
    const char* test = "rtmp://10.74.34.97/live/room";
    if (0 != openUrl(test)) {
        uninitialzie();
        return -1;
    }

    while (!_kbhit()) {
        //std::cout << "Hit me!!" << std::endl;
        Sleep(50);
    }

    std::cout << "you input:" << _getch() << std::endl;

    uninitialzie();
    return 0;
}