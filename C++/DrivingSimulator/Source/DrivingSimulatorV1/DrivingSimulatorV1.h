#ifndef DRIVING_SIMULATOR_V1_H
#define DRIVING_SIMULATOR_V1_H

#include "MainApplication.h"

class DrivingSimulatorV1 : public MainApplication
{
	public:
	DrivingSimulatorV1();
	virtual ~DrivingSimulatorV1();
	virtual void createScene1();
	virtual void createScene2();

	protected:
	Ogre::SceneNode* worldNode;
	Ogre::SceneNode* carNode;
	Ogre::SceneNode* pointerNode;
	Ogre::Real speed;
	Ogre::Real cameraRotationOffset;
	Ogre::uint8 cameraMode;
	Ogre::uint8 keyState[256];

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // DRIVING_SIMULATOR_V1_H
