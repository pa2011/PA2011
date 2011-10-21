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

class MainApplication : public Ogre::WindowEventListener, public Ogre::FrameListener
{
	public:
	MainApplication();
	virtual ~MainApplication();
	void init();
	void createScene();
	void start();

	protected:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	private:
	Ogre::Root* rootNode;
	Ogre::RenderWindow* renderWindow;
	Ogre::SceneManager* sceneManager;
	Ogre::Camera* camera;
	Ogre::SceneNode* carNode;
};

#endif // MAINAPPLICATION_H
