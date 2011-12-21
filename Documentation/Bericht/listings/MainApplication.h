#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#endif

#include <string.h>

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreException.h>
#include <OgreNode.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "UdpListener.h"

class MainApplication : public Ogre::WindowEventListener, public Ogre::FrameListener
{
	public:
	MainApplication();
	virtual ~MainApplication();
	virtual void start();

	protected:
	Ogre::Root* rootNode;
	Ogre::RenderWindow* renderWindow;
	Ogre::SceneManager* sceneManager;
	Ogre::Camera* camera;
	OIS::InputManager* inputManager;
	OIS::Mouse* mouse;
	OIS::Keyboard* keyboard;

	virtual void windowResized(Ogre::RenderWindow* window);
	virtual void windowClosed(Ogre::RenderWindow* window);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;

};

#endif // MAINAPPLICATION_H
