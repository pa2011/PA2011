#ifndef Scene3_H
#define Scene3_H

#include "MainApplication.h"

class Scene3 : public MainApplication
{
	public:
	Scene3();
	virtual ~Scene3();
	virtual void createScene();

	protected:
	Ogre::SceneNode* worldNode;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // Scene3_H
