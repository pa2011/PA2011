#ifndef UdpTrace_util_h
#define UdpTrace_util_h

#define true 1
#define false 0

#define LOCALHOST "127.0.0.1"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define OS_WINDOWS
#endif

int setupVlc(const char* exe, int port, const char* password, const char* channel, const char* video);
int sendMessage(const char * message);
int play();
int pause();
void execProcess(char* szExe, char* szArgs);

#endif
