#ifndef SCENE5_H
#define SCENE5_H

#include "MainApplication.h"

class Scene5 : public MainApplication
{
	public:
	Scene5();
	virtual ~Scene5();
	virtual void createScene();

	protected:
	OgreOde::World* world;
	OgreOde::Space* space;
	OgreOde::StepHandler* stepper;
	OgreOde::InfinitePlaneGeometry* ground;
	OgreOde::Geometry* geometry;

	Ogre::SceneNode* planeNode;
	Ogre::SceneNode* cubeNode;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // SCENE5_H
