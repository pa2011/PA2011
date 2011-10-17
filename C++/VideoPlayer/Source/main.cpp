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

#define BUFFER_LENGTH 1024

int main(int argc, char** argv)
{
	int udpPort;
	int socketId;
	struct sockaddr_in server;
	char buf[BUFFER_LENGTH];
	
	if(argc < 7)
	{
		fprintf(stderr, "Usage: %s udp-port vlc-path vlc-port vlc-password channel-name video-path\n", argv[0]);
		return 1;
	}
	
	udpPort = atoi(argv[1]);

	#ifdef OS_WINDOWS
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
		{
			fprintf(stderr, "Could not start WinSock.");
			return 1;
		}
	#endif
	
	socketId = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketId < 0)
	{
		fprintf(stderr, "Could not open socket.\n");
		return 1;
	}
	
	memset(&server, 0, sizeof(server));
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(udpPort);
	
	if(bind(socketId, (sockaddr *)&server, sizeof(server)) < 0) 
	{
		fprintf(stderr, "Could not bind to socket.\n");
		return 1;
	}

	if(setupVlc(argv[2], atoi(argv[3]), argv[4], argv[5], argv[6]) != 0)
	{
		return 1;
	}

	printf("Listening on port %d.\n", udpPort);
	while(true)
	{
		memset(&buf, 0, BUFFER_LENGTH);
		
		int msgLength = (int)recv(socketId, buf, BUFFER_LENGTH, 0);
		if (msgLength < 0)
		{
			fprintf(stderr, "Error in call to recv.\n");
			return 1;
		}
		
		char* str = "0";
		char* thr = "0";
		char* p = strtok(buf, ",");
		if(p != NULL)
		{
			str = p;
			p = strtok(NULL, ",");
			if(p != NULL)
			{
				thr = p;
			}
		}

		int steerValue = atoi(str);
		int throttleValue = atoi(thr);

		printf("Steer:%d Throttle:%d\n", steerValue, throttleValue);

		if(throttleValue <= -20000)
		{
			if(play() != 0)
				return 1;
		}
		else if(throttleValue >= 20000)
		{
			if(pause() != 0)
				return 1;
		}
	}
	
	return 0;
}






