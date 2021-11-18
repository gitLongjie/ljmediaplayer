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


int main() {
    std::cout << "hello c++" << std::endl;
    if (0 != initialize(error_callback)) {
        return -1;
    }

    const char* test = "rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp";
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