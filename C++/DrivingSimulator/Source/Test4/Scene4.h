#ifndef SCENE4_H
#define SCENE4_H

#include "MainApplication.h"

class Scene4 : public MainApplication
{
	public:
	Scene4();
	virtual ~Scene4();
	virtual void createScene();

	protected:
	Ogre::SceneNode* worldNode;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // SCENE4_H
