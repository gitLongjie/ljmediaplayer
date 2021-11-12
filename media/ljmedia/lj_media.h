#ifndef ljmedia_lj_media_h_
#define ljmedia_lj_media_h_

#include "ljmedia/config.h"

#if __cplusplus
extern "C" {
#endif

typedef void(*errorCallback)(int, const char*);

MEDIA_EXPORT int initialize(errorCallback callback);
MEDIA_EXPORT void uninitialzie();

MEDIA_EXPORT int openUrl(const char* szUrl);

#if __cplusplus
}
#endif

#endif // !ljmedia_lj_media_h_