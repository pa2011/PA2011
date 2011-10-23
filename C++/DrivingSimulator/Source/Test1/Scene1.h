#ifndef SCENE1_H
#define SCENE1_H

#include "MainApplication.h"

class Scene1 : public MainApplication
{
	public:
	Scene1();
	virtual ~Scene1();
	virtual void createScene();

	protected:
	Ogre::Real rotationSpeed;
	Ogre::SceneNode* carNode;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // SCENE1_H
