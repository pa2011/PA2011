#ifndef SCENE3_H
#define SCENE3_H

#include "MainApplication.h"

class Scene3 : public MainApplication
{
	public:
	Scene3();
	virtual ~Scene3();
	virtual void createScene();

	protected:
	Ogre::SceneNode* worldNode;
	Ogre::SceneNode* carNode;
	Ogre::SceneNode* ethNode;
	Ogre::Real speed;
	Ogre::Real cameraRotationOffset;
	Ogre::uint8 cameraMode;
	Ogre::uint8 keyState[256];

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // SCENE3_H
