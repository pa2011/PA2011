#include "UdpListener.h"

int UdpListener::udpPortIn = 0;
int UdpListener::udpPortOut = 0;
char UdpListener::remoteAddress[255] = "";

int UdpListener::timeStamp = 0;
Ogre::Real UdpListener::steer = 0;
Ogre::Real UdpListener::throttle = 0;
Ogre::Real UdpListener::speed = 0;

void UdpListener::loop(void* nothing)
{
	int socketIdIn;
	struct sockaddr_in addressIn;
	char bufferIn[BUFFER_LENGTH];

	int socketIdOut;
	struct sockaddr_in addressOut;
	char bufferOut[BUFFER_LENGTH];

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not start WinSock", "UdpListener::UdpListener");
	}
	#endif

    // create in socket
	socketIdIn = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdIn < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not open in socket", "UdpListener::UdpListener");
	}
	memset(&addressIn, 0, sizeof(addressIn));
	addressIn.sin_family = AF_INET;
	addressIn.sin_addr.s_addr = INADDR_ANY;
	addressIn.sin_port = htons(udpPortIn);
	if(bind(socketIdIn, (sockaddr *)&addressIn, sizeof(addressIn)) < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not bind to in socket", "UdpListener::UdpListener");
	}
	std::cout << "Listening on port " << udpPortIn << std::endl;

	// create out socket
	socketIdOut = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdOut < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not open out socket", "UdpListener::UdpListener");
	}
	memset(&addressOut, 0, sizeof(addressOut));
	addressOut.sin_family = AF_INET;
	addressOut.sin_addr.s_addr = inet_addr(remoteAddress);
	addressOut.sin_port = htons(udpPortOut);
	if(connect(socketIdOut, (sockaddr *)&addressOut, sizeof(addressOut)) < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not connect to out socket", "UdpListener::UdpListener");
	}
	std::cout << "Connected to " << remoteAddress << ":" << udpPortOut << std::endl;

	while(true)
	{
	    // read from udp socket

		memset(&bufferIn, 0, BUFFER_LENGTH);

		int msgLength = (int)recv(socketIdIn, bufferIn, BUFFER_LENGTH, 0);
		if (msgLength < 0)
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Error in recv function", "UdpListener::capture");
		}

        char tms[32];
        char str[32];
        char thr[32];
        memset(&tms, 0, sizeof(tms));
        memset(&str, 0, sizeof(str));
        memset(&thr, 0, sizeof(thr));

        char* p = strtok(bufferIn, ";");
        if(p != NULL)
        {
            sprintf(tms, "%s", p);
            p = strtok(NULL, ";");
            if(p != NULL)
            {
                sprintf(str, "%s", p);
                p = strtok(NULL, ";");
                if(p != NULL)
                {
                    sprintf(thr, "%s", p);
                }
            }
        }

        timeStamp = atoi(tms);
		int steerValue = atoi(str);
		int throttleValue = atoi(thr);

		steer = (Ogre::Real)steerValue / 32768;
		throttle = (Ogre::Real)throttleValue / -32768;




		// write to udp socket

        memset(&bufferOut, 0, BUFFER_LENGTH);
        sprintf(bufferOut, "%d;%.2f%\n", timeStamp, speed);

        msgLength = (int)sendto(socketIdOut, bufferOut, BUFFER_LENGTH, 0, (sockaddr *)&addressOut, sizeof(addressOut));
        if (msgLength < 0)
        {
            throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Error in sendto function", "UdpListener::capture");
		}
	}
}

void UdpListener::startUdpListener(int portIn, int portOut, const char* address)
{
    udpPortIn = portIn;
    udpPortOut = portOut;
    memset(&remoteAddress, 0, sizeof(remoteAddress));
    sprintf(remoteAddress, "%s", address);

	_beginthread(UdpListener::loop, 0, NULL);
}
