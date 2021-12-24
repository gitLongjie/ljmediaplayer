#include <iostream>
#include <vector>
#include <set>

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
  
     if (0 != initialize(error_callback)) {
         return -1;
     }
      
#if 0

     WSAData wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		
		return-1;
	}

    std::set<SOCKET> connect_sockets;
    int ret = 0;
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	
	struct sockaddr_in service;
	memset(&service, 0, sizeof(struct sockaddr_in));
	service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("10.74.34.79");

    int error = 0;
	service.sin_port = htons(1935);
    ret = connect(sock, (struct sockaddr*)&service, sizeof(sockaddr));
    if (ret < 0) {
        error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
			WSACleanup();
			return -1;
        }
       
    }

	unsigned long ul = 1;
	ret = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);

   // connect_sockets.emplace(sock);


	fd_set wset;
    fd_set rset;
	struct timeval tval;

	
    while (true)
    {
		FD_ZERO(&wset);
        for (auto sock : connect_sockets)
        {
            FD_SET(sock, &wset);
        }
		
		FD_ZERO(&rset);
		FD_SET(sock, &rset);

		tval.tv_sec = 2;
		tval.tv_usec = 0;

		ret = ::select(2, &rset, nullptr, nullptr, &tval);
		if (SOCKET_ERROR == ret) {
			error = WSAGetLastError();
            WSACleanup();
			return -1;
        }
        else if (0 == ret) {
            continue;
        }

        if (FD_ISSET(sock, &wset)) {
            connect_sockets.erase(sock);
            std::cout << "write\n";
            send(sock, "test", sizeof("test"), 0);
        }

        if (FD_ISSET(sock, &rset)) {
            char buffer[1024] = { 0 };
            int len = ::recv(sock, buffer, sizeof(buffer), 0);
            if (0 == len) {
                break;
            }
            std::cout << len << std::endl;
        }
    }
    WSACleanup();
    return 0;
#endif

    std::string str = std::string("a", 1);
    unsigned int n = sizeof(FLVTagHeader);
    // const char* test = "rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp";
    //const char* test = "rtmp://ns8.indexforce.com/home/mystream";
    // const char* test = "rtmp://10.74.34.97/live/room";
    const char* test = "rtmp://39.107.224.142/live/stream1";
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