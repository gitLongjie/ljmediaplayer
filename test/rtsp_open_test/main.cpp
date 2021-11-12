#include <iostream>

#include <ljmedia/lj_media.h>

int main() {
    std::cout << "hello c++" << std::endl;
    if (0 != initialize()) {
        return -1;
    }

    const char* test = "rtsp://admin:Zswbimvr@xxzx2.izsw.net:50001/Streaming/Channels/101";
    if (0 != openUrl(test)) {
        return -1;
    }

    int n = 0;
    std::cin >> n;
    std::cout << "you input:" << n << std::endl;;

    uninitialzie();
    return 0;
}