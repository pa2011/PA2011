#include "util.h"

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef OS_WINDOWS
	#include <winsock2.h>
	#include <stdio.h>
	#include <stdlib.h>
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
#endif

char vlcExe[255];
int vlcPort = 0;
char vlcPassword[255];
char channelName[255];
char videoFile[255];

int socketId;
int isPlaying = 0;

int setupVlc(const char* exe, int port, const char* password, const char* channel, const char* video)
{
	struct sockaddr_in server;
	struct hostent* hp;
	char* hostName;

	strcpy(vlcExe, exe);
	vlcPort = port;
	strcpy(vlcPassword, password);
	strcpy(channelName, channel);
	strcpy(videoFile, video);

	char arguments[1024];
	memset(&arguments, 0, sizeof(arguments));
	sprintf(arguments, " --extraintf telnet --telnet-port %d --telnet-password %s", vlcPort, vlcPassword);

	execProcess(vlcExe, arguments);

	if ((hp = gethostbyname(LOCALHOST)) == 0)
	{
		fprintf(stderr, "Could not find host: '%s'.\n", LOCALHOST);
		return 1;
	}

	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	server.sin_port = htons(vlcPort);

	/* grab an Internet domain socket */
	if ((socketId = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Could not create socket.\n");
		return 1;
	}

	/* connect to PORT on HOST */
	if (connect(socketId, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Could not connect to server.\n");
		return 1;
	}
	
	char message[1024];

	memset(&message, 0, sizeof(message));
	sprintf(message, "%s\n", vlcPassword);
	if(sendMessage(message) != 0)
		return 1;

	memset(&message, 0, sizeof(message));
	sprintf(message, "del %s\n", channelName);
	if(sendMessage(message) != 0)
		return 1;

	memset(&message, 0, sizeof(message));
	sprintf(message, "new %s broadcast enabled\n", channelName);
	if(sendMessage(message) != 0)
		return 1;


	memset(&message, 0, sizeof(message));
	sprintf(message, "setup %s input \"%s\"\n", channelName, videoFile);
	if(sendMessage(message) != 0)
		return 1;

	play();
	Sleep(100);
	pause();

	printf("VLC is now up and running.\n");

	return 0;
}

int sendMessage(const char* message)
{
	char * play = "test\ncontrol channel1 play\n";
	if (send(socketId, message, strlen(message), 0) == -1)
	{
		fprintf(stderr, "Could not send message: '%s'.\n", message);
		return 1;
	}
	return 0;
}

int play()
{
	if(!isPlaying)
	{
		isPlaying = true;
		char message[255];
		sprintf(message, "control %s play\n", channelName);
		return sendMessage(message);
	}
	return 0;
}

int pause()
{
	if(isPlaying)
	{
		isPlaying = false;
		char message[255];
		sprintf(message, "control %s pause\n", channelName);
		return sendMessage(message);
	}
	return 0;
}

void execProcess(char* szExe, char* szArgs)
{
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    if(CreateProcessA(szExe, szArgs, 0, 0, FALSE, 0, 0, 0, LPSTARTUPINFOA(&si), &pi))
    {
        // optionally wait for process to finish
        // WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

