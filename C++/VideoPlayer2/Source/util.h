#ifndef UdpTrace_util_h
#define UdpTrace_util_h

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WINDOWS
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef OS_WINDOWS

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#endif

#define true 1
#define false 0

#define BUFFER_LENGTH 1024

#define LOCALHOST "127.0.0.1"
#define DEFAULT_UDP_IN_PORT 52284
#define DEFAULT_UDP_OUT_PORT 52285

#define SEPARATING_CHARACTER ','

int setupSockets(int udpPortIn, int udpPortOut, const char* remoteAddress);
int readFromSocket();
int writeToSocket(float speed, float videoTimePos);
int getSteerValue();
int getThrottleValue();
int startMPlayer(const char* mPlayerPath, const char* videoPath, float initialPosition);
int isPlaying();
int setSpeed(float speed);
int playVideo(float speed);
int pauseVideo();
void refreshTimePos();
float getTimePos();
int seek(float position);
int sendMessage(const char* message);
int readMessage(char* buffer, int wait);
void execProcess(const char* szExe, const char* szArgs, HANDLE hStdIn, HANDLE hStdOut, HANDLE hStdErr);

#endif
