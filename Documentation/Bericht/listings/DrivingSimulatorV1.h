#ifndef DRIVING_SIMULATOR_V1_H
#define DRIVING_SIMULATOR_V1_H

#include "MainApplication.h"

class DrivingSimulatorV1 : public MainApplication
{
	public:
	DrivingSimulatorV1();
	virtual ~DrivingSimulatorV1();
	void setAllowReverse(Ogre::uint8 allowReverse);
	void setShowSteeringWheel(Ogre::uint8 showSteeringWheel);
	void createCar();
	void createScene1();
	void createScene2();

	protected:
	Ogre::SceneNode* carNode;
	Ogre::SceneNode* cockpitNode;
	Ogre::SceneNode* pointerNode;
	Ogre::SceneNode* steeringWheelNode;
	Ogre::Real keyboardSteer;
	Ogre::Real speed;
	Ogre::Real cameraRotationOffset;
	Ogre::uint8 cameraMode;
	Ogre::uint8 keyState[256];
	Ogre::uint8 gear;
	Ogre::uint8 allowReverse;
	Ogre::uint8 showSteeringWheel;

	private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // DRIVING_SIMULATOR_V1_H
