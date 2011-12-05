#include "util.h"

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

int main(int argc, char** argv)
{
	float videoStart;
	float videoEnd;

	float recordedSpeedKmh;

	int udpPortIn;
	int udpPortOut;
	char remoteAddress[255];

	if(argc != 6 && argc != 9)
	{
		fprintf(stderr, "Usage: %s mplayer-path video-path video-start video-end recorded-speed-kmh [ udp-in-port udp-out-port remote-address]\n", argv[0]);
		return 1;
	}

	if(argc == 6)
	{
		printf("No ports specified, using default ports: %d, %d and remote address: %s\n", DEFAULT_UDP_IN_PORT, DEFAULT_UDP_OUT_PORT, LOCALHOST);
		udpPortIn = DEFAULT_UDP_IN_PORT;
		udpPortOut = DEFAULT_UDP_OUT_PORT;
		memset(&remoteAddress, 0, sizeof(remoteAddress));
		sprintf(remoteAddress, "%s", LOCALHOST);
	}
	else
	{
		udpPortIn = atoi(argv[6]);
		udpPortOut = atoi(argv[7]);
		memset(&remoteAddress, 0, sizeof(remoteAddress));
		sprintf(remoteAddress, "%s", argv[8]);
	}

	videoStart = atof(argv[3]);
	videoEnd = atof(argv[4]);
	recordedSpeedKmh = atof(argv[5]);

	if(startMPlayer(argv[1], argv[2], videoStart) != 0)
	{
		fprintf(stderr, "Could not start MPlayer.");
		return 1;
	}

	if(setupSockets(udpPortIn, udpPortOut, remoteAddress) != 0)
	{
		fprintf(stderr, "Could not setup sockets.");
		return 1;
	}

	float speed = 1.0;
	int oldTimeStamp = 0;
	while(true)
	{
		// update throttle value
		readFromSocket();

		float tpf = 0.0;
		if(oldTimeStamp > 0)
		{
			tpf = (float)(getTimeStamp() - oldTimeStamp) / 1000;
		}
		oldTimeStamp = getTimeStamp();

		// calculate acceleration and update speed
		float acceleration = (float)getThrottleValue() / -32768;
		speed *= pow(0.9, tpf);
		if(acceleration > 0)
			speed += acceleration / 5.0 * tpf;
		else
			speed += acceleration / 4.0 * tpf;

		// calculate real world speed (in km/h)
		float actualSpeed = 0;
		if(speed < 0.15)
		{
			speed = 0.15;
			if(isPlaying())
				pauseVideo();
		}
		else
		{
			actualSpeed = speed * recordedSpeedKmh;
			playVideo(speed);
		}

		// get current position in video
		refreshTimePos();
		if(getTimePos() > videoEnd)
		{
			seek(videoStart);
			//speed = 0.0;
		}

		// write data out to receiver
		writeToSocket(getTimeStamp(), actualSpeed, getTimePos());
	}

	return 0;
}

