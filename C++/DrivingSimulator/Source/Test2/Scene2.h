#ifndef SCENE2_H
#define SCENE2_H

#include "MainApplication.h"

class Scene2 : public MainApplication
{
	public:
	Scene2();
	virtual ~Scene2();
	virtual void createScene();

	protected:
	Ogre::SceneNode* worldNode;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // SCENE2_H
