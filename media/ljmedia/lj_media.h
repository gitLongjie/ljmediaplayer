#ifndef ljmedia_lj_media_h_
#define ljmedia_lj_media_h_

#include "ljmedia/config.h"

#if __cplusplus
extern "C" {
#endif

MEDIA_EXPORT int initialize();
MEDIA_EXPORT void uninitialzie();

MEDIA_EXPORT int openUrl(const char* szUrl);

#if __cplusplus
}
#endif

#endif // !ljmedia_lj_media_h_