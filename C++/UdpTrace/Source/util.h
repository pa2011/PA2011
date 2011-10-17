#ifndef UdpTrace_util_h
#define UdpTrace_util_h

#define true 1
#define false 0

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define OS_WINDOWS
#endif

#endif
