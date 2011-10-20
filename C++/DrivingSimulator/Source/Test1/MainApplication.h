#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <OgreRoot.h>
#include <OgreConfigFile.h>

class MainApplication
{
	public:
	MainApplication();
	virtual ~MainApplication();
	bool go();

	private:
	Ogre::Root* rootNode;
	Ogre::RenderWindow* window;
};

#endif // MAINAPPLICATION_H
