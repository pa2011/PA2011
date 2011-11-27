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
	int udpPortIn;
	int socketIdIn;
	struct sockaddr_in serverIn;

	int udpPortOut;
	int socketIdOut;
	struct sockaddr_in serverOut;

	char bufIn[BUFFER_LENGTH];
	char bufOut[BUFFER_LENGTH];
	float videoStart;
	float videoEnd;

    if(argc != 5 && argc != 7)
    {
        fprintf(stderr, "Usage: %s mplayer-path video-path video-start video-end [ udp-in-port udp-out-port ]\n", argv[0]);
        return 1;
    }

	if(argc == 5)
	{
		printf("No ports specified, using default ports: %d, %d\n", DEFAULT_UDP_IN_PORT, DEFAULT_UDP_OUT_PORT);
		udpPortIn = DEFAULT_UDP_IN_PORT;
		udpPortOut = DEFAULT_UDP_OUT_PORT;
	}
	else
	{
	    udpPortIn = atoi(argv[5]);
	    udpPortOut = atoi(argv[6]);
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

	// create in socket
	socketIdIn = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdIn < 0)
	{
		fprintf(stderr, "Could not open in socket.\n");
		return 1;
	}
	memset(&serverIn, 0, sizeof(serverIn));
	serverIn.sin_family = AF_INET;
	serverIn.sin_addr.s_addr = INADDR_ANY;
	serverIn.sin_port = htons(udpPortIn);
	if(bind(socketIdIn, (sockaddr *)&serverIn, sizeof(serverIn)) < 0)
	{
		fprintf(stderr, "Could not bind to in socket.\n");
		return 1;
	}
	printf("Listening on port %d.\n", udpPortIn);


	// create out socket
	socketIdOut = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdOut < 0)
	{
		fprintf(stderr, "Could not open out socket.\n");
		return 1;
	}
	memset(&serverOut, 0, sizeof(serverOut));
	serverOut.sin_family = AF_INET;
	serverOut.sin_addr.s_addr = INADDR_ANY;
	serverOut.sin_port = htons(udpPortOut);
	if(bind(socketIdOut, (sockaddr *)&serverOut, sizeof(serverOut)) < 0)
	{
		fprintf(stderr, "Could not bind to out socket.\n");
		return 1;
	}
	printf("Writing to port %d.\n", udpPortOut);


    float speed = 0.0;
	while(true)
	{
		// read message
		memset(&bufIn, 0, BUFFER_LENGTH);
		int msgLength = (int)recv(socketIdIn, bufIn, BUFFER_LENGTH, 0);
		if (msgLength < 0)
		{
			fprintf(stderr, "Error in call to recv.\n");
			return 1;
		}

		// send message
		memset(&bufOut, 0, BUFFER_LENGTH);

		int msgLength = (int)sendto(socketIdOut, bufOut, BUFFER_LENGTH, NULL, 0);
		if (msgLength < 0)
		{
			fprintf(stderr, "Error in call to recv.\n");
			return 1;
		}

		char* str = "0";
		char* thr = "0";
		char* p = strtok(bufIn, ",");
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

		//printf("Position: %.2lf sec.\n", getTimePos());
	}

	return 0;
}






