#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <process.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
#endif

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreException.h>

#define LOCALHOST "127.0.0.1"
#define DEFAULT_UDP_PORT 52284
#define BUFFER_LENGTH 1024

class UdpListener
{
	public:
	static Ogre::Real steer;
	static Ogre::Real throttle;
	static void startUdpListener();

	protected:
    static void listenToUdpPort(void* nothing);
};

#endif // UDPLISTENER_H
