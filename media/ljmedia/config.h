#ifndef ljmedia_config_h_
#define ljmedia_config_h_

#if defined _WIN32 || defined __CYGWIN__

  #ifdef LJ_MEDIA_EXPORT
    #ifdef __GNUC__
      #define MEDIA_EXPORT __attribute__((dllexport))
    #else
      #define MEDIA_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif

  #else

    #ifdef __GNUC__
      #define MEDIA_EXPORT __attribute__((dllimport))
    #else
      #define MEDIA_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif

  #define DLL_LOCAL

#else

  #if __GNUC__ >= 4
#ifdef #ifdef LJ_MEDIA_EXPORT
    #define MEDIA_EXPORT __attribute__ ((visibility("default")))
#else
#define MEDIA_EXPORT __attribute__ ((visibility("hide")))
#endif
  #else
    #define MEDIA_EXPORT
  #endif

#endif

#endif // !ljmedia_config_h_