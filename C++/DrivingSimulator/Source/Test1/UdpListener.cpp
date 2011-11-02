#include "UdpListener.h"

void listenToUdpPort(void* nothing)
{
	int udpPort = DEFAULT_UDP_PORT;
	int socketId;
	struct sockaddr_in server;
	char buf[BUFFER_LENGTH];

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not start WinSock", "UdpListener::UdpListener");
	}
	#endif

	socketId = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketId < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not open socket", "UdpListener::UdpListener");
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(udpPort);

	if(bind(socketId, (sockaddr *)&server, sizeof(server)) < 0)
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not bind to socket", "UdpListener::UdpListener");
	}

	std::cout << "Listening on port " << udpPort << std::endl;

	while(true)
	{
		memset(&buf, 0, BUFFER_LENGTH);

		int msgLength = (int)recv(socketId, buf, BUFFER_LENGTH, 0);
		if (msgLength < 0)
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Error in call to recv", "UdpListener::capture");
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

		Ogre::Real steer = (Ogre::Real)steerValue / 32768;
		Ogre::Real throttle = (Ogre::Real)throttleValue / -32768;

		std::cout << throttle << std::endl;
	}

}

void startUdpListener()
{
	_beginthread(listenToUdpPort, 0, NULL);
}



