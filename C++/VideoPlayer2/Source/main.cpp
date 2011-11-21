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
	float videoStart;
	float videoEnd;

    if(argc < 5)
    {
        fprintf(stderr, "Usage: %s mplayer-path video-path video-start video-end [ udp-port ]\n", argv[0]);
        return 1;
    }

	if(argc < 6)
	{
		printf("No port specified, using default port %d\n", DEFAULT_UDP_PORT);
		udpPort = DEFAULT_UDP_PORT;
	}
	else
	{
	    udpPort = atoi(argv[5]);
	}

	videoStart = atof(argv[3]);
	videoEnd = atof(argv[4]);

    #ifdef OS_WINDOWS
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
		{
			fprintf(stderr, "Could not start WinSock.");
			return 1;
		}
	#endif

	if(startMPlayer(argv[1], argv[2], videoStart) != 0)
	{
        fprintf(stderr, "Could not start MPlayer.");
		return 1;
	}

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

	printf("Listening on port %d.\n", udpPort);

    float speed = 0.0;
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

		float acceleration = (float)throttleValue / -32768;
		speed = speed*0.997;
		if(acceleration > 0)
			speed += acceleration/150.0;
		else
			speed += acceleration/50.0;

		if(speed < 0.15)
		{
		    speed = 0.15;
		    if(isPlaying())
                pauseVideo();

			printf("Speed: %.2lf km/h\n", 0);
		}
		else
		{
			printf("Speed: %.2lf km/h\n", speed*100);
            playVideo(speed);
		}

		refreshTimePos();
		if(getTimePos() > videoEnd)
		{
			seek(videoStart);
			//speed = 0.0;
		}
	}

	return 0;
}






