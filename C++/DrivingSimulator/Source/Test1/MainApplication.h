#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

class MainApplication
{
	public:
	MainApplication();
	virtual ~MainApplication();
	void init();
	void createScene();
	void run();

	private:
	Ogre::Root* rootNode;
	Ogre::RenderWindow* renderWindow;
	Ogre::SceneManager* sceneManager;
	Ogre::Camera* camera;
};

#endif // MAINAPPLICATION_H
