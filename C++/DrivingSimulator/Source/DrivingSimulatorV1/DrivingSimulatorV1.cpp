#include "DrivingSimulatorV1.h"

#define THIRD_PERSON 0
#define COCKPIT 1
#define ALLOW_REVERSE true
#define SHOW_STEERING_WHEEL true
#define THROTTLE_THRESHOLD 0.05
#define NEUTRAL 0
#define DRIVE 1
#define REVERSE 2

DrivingSimulatorV1::DrivingSimulatorV1()
{
    // initialize attributes
    cameraRotationOffset = 0;
    cameraMode = COCKPIT;
    keyState[256] = {0};
    keyboardSteer = 0;
    gear = NEUTRAL;
    speed = 0;
}

DrivingSimulatorV1::~DrivingSimulatorV1()
{

}

void DrivingSimulatorV1::createCar()
{
	// load car
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(4, 4, 4);

	// load Cockpit
	Ogre::Entity* cockpit = sceneManager->createEntity("MiniCockpit.mesh");
	cockpitNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	cockpitNode->attachObject(cockpit);
	cockpitNode->scale(0.05, 0.05, 0.05);

	Ogre::Entity* pointer = sceneManager->createEntity("MiniCockpitPointer.mesh");
	pointerNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	pointerNode->attachObject(pointer);
	pointerNode->scale(0.02, 0.02, 0.02);

	Ogre::Entity* steeringWheel = sceneManager->createEntity("MiniCockpitSteeringWheel.mesh");
	steeringWheelNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	steeringWheelNode->attachObject(steeringWheel);
	steeringWheelNode->scale(0.03, 0.03, 0.03);

	// setup camera
	camera->setFOVy(Ogre::Degree(70));
}

void DrivingSimulatorV1::createScene1() // city
{
	// create world node
	Ogre::SceneNode* worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* cityWorld = sceneManager->createEntity("CityWorld.mesh");
	worldNode->scale(0.05, 0.05, 0.05);
	worldNode->attachObject(cityWorld);

	// create ETH Node
	Ogre::SceneNode* ethNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* eth = sceneManager->createEntity("ETH.mesh");
	ethNode->attachObject(eth);
	ethNode->scale(1.3, 1.3, 1.3);
	ethNode->setPosition(428, 0, 235);
	ethNode->yaw(Ogre::Degree(210));

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(-0.5, -0.5, 0.5));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.7, 0.7, 0.7));

	// set car to initial position and orientation
	carNode->setPosition(584, 0, 121);
	carNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-4.5), Ogre::Vector3::UNIT_Y));
}

void DrivingSimulatorV1::createScene2() // tunnels
{
	// create world node
	Ogre::SceneNode* worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* map = sceneManager->createEntity("MountainMap.mesh");
	worldNode->scale(0.05, 0.05, 0.05);
	worldNode->attachObject(map);

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(-0.5, -0.5, 0.5));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.7, 0.7, 0.7));

	// set car to initial position and orientation
	carNode->setPosition(309, 0, -604);
	carNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
}

bool DrivingSimulatorV1::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	// capture input devices
	if(inputManager)
	{
		keyboard->capture();
		mouse->capture();
	}

    // abort scene if window has been closed or escape button has been hit
	if(renderWindow->isClosed() || keyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	// decrease speed (air resistance, etc...)
	if(speed > 0)
        speed *= Ogre::Math::Pow(0.92, evt.timeSinceLastFrame);
    else
        speed *= Ogre::Math::Pow(0.7, evt.timeSinceLastFrame);

	// accelerate / brake car
    if(gear == DRIVE)
    {
        // udp input
        if(UdpListener::throttle >= 0)
        {
            speed += UdpListener::throttle * 9 * evt.timeSinceLastFrame;
        }
        else
        {
            speed += UdpListener::throttle * 30 * evt.timeSinceLastFrame;
        }

        // keyboard input
        if(keyboard->isKeyDown(OIS::KC_UP))
        {
            speed += 9 * evt.timeSinceLastFrame;
        }
        if(keyboard->isKeyDown(OIS::KC_DOWN))
        {
            speed -= 30 * evt.timeSinceLastFrame;
        }

        if(speed < 0)
        {
            speed = 0;
        }
    }
    else if(gear == REVERSE)
    {
        // udp input
        if(UdpListener::throttle <= 0)
        {
            speed += UdpListener::throttle * 9 * evt.timeSinceLastFrame;
        }
        else
        {
            speed += UdpListener::throttle * 30 * evt.timeSinceLastFrame;
        }

        // keyboard input
        if(keyboard->isKeyDown(OIS::KC_UP))
        {
            speed += 30 * evt.timeSinceLastFrame;
        }
        if(keyboard->isKeyDown(OIS::KC_DOWN))
        {
            speed -= 9 * evt.timeSinceLastFrame;
        }

        if(speed > 0)
            speed = 0;
    }
    else
    {
        if(UdpListener::throttle > THROTTLE_THRESHOLD || keyboard->isKeyDown(OIS::KC_UP))
            gear = DRIVE;
        else if(ALLOW_REVERSE && UdpListener::throttle < -THROTTLE_THRESHOLD || keyboard->isKeyDown(OIS::KC_DOWN))
            gear = REVERSE;
    }

    if(speed == 0 && Ogre::Math::Abs(UdpListener::throttle) < THROTTLE_THRESHOLD && !keyboard->isKeyDown(OIS::KC_UP) && !keyboard->isKeyDown(OIS::KC_DOWN))
        gear = NEUTRAL;

	// update speed information for udp socket
	UdpListener::speed = Ogre::Math::Abs(speed) * 1.1;

	// calculate steer intensity
	Ogre::Real normalizedSpeed = Ogre::Math::Abs(speed / 180);
	Ogre::Real steerIntensity = 100 / (100 * (Ogre::Math::Pow(normalizedSpeed, 2)) + 1) * Ogre::Math::Pow(normalizedSpeed, 1.5);

	// rotate car by udp input
	carNode->yaw(Ogre::Degree(UdpListener::steer * -5 * evt.timeSinceLastFrame * speed));
    cameraRotationOffset += UdpListener::steer * 90 * evt.timeSinceLastFrame;

    // rotate car by keyboard input
    if(keyboard->isKeyDown(OIS::KC_LEFT))
    {
        keyboardSteer += 270 * evt.timeSinceLastFrame;
    }
    if(keyboard->isKeyDown(OIS::KC_RIGHT))
    {
        keyboardSteer -= 270 * evt.timeSinceLastFrame;
    }

    if(!keyboard->isKeyDown(OIS::KC_LEFT) && !keyboard->isKeyDown(OIS::KC_RIGHT))
    {
        if(keyboardSteer > 0)
            keyboardSteer -= std::min(270 * evt.timeSinceLastFrame, keyboardSteer);
        else if(keyboardSteer < 0)
            keyboardSteer += std::min(270 * evt.timeSinceLastFrame, keyboardSteer * -1);
    }

    if(keyboardSteer < -90)
        keyboardSteer = -90;
    else if(keyboardSteer > 90)
        keyboardSteer = 90;

    if(speed != 0)
    {
        carNode->yaw(Ogre::Degree(steerIntensity * keyboardSteer * evt.timeSinceLastFrame * Ogre::Math::Abs(speed) / speed));
        cameraRotationOffset -= keyboardSteer * 0.3 * evt.timeSinceLastFrame * steerIntensity * Ogre::Math::Abs(speed) / speed;
    }

	// change camera mode
	if(keyboard->isKeyDown(OIS::KC_V))
	{
		if(!keyState[OIS::KC_V])
		{
			cameraMode = 1 - cameraMode;
			if(cameraMode == THIRD_PERSON)
			{
				cameraRotationOffset = 0;
			}

		}
		keyState[OIS::KC_V] = true;
	}
	else
	{
		keyState[OIS::KC_V] = false;
	}

	// update car position
	Ogre::Real xMove = Ogre::Math::Sin(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	Ogre::Real zMove = Ogre::Math::Cos(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	carNode->translate(xMove, 0, zMove);

	// update camera
	if(cameraMode == COCKPIT)
	{
		// position camera
        Ogre::Vector3 cameraOffset(1.3, 4.0, 0.7);
        camera->setOrientation(carNode->getOrientation() * Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y));
        camera->setPosition(carNode->getPosition() + carNode->getOrientation() * cameraOffset);

		// position car cockpit
		Ogre::Vector3 cockpitOffset(0, 2, 0.7);
		cockpitNode->setPosition(carNode->getPosition() + carNode->getOrientation() * cockpitOffset);
		cockpitNode->setOrientation(carNode->getOrientation());
		cockpitNode->yaw(Ogre::Degree(180));

        // fix inaccuracy of carNode's orientation and position by assigning it it's own transformations
        carNode->setOrientation(carNode->getOrientation());
        carNode->setPosition(carNode->getPosition());

        // position speed pointer
        Ogre::Vector3 pointerOffset(0.0, 2.7, 2.9);
		Ogre::Quaternion pointerPitch(Ogre::Degree(15), Ogre::Vector3::UNIT_X);
        Ogre::Quaternion pointerRoll(Ogre::Degree(45 + Ogre::Math::Abs(UdpListener::speed * 2.25)), Ogre::Vector3::UNIT_Z);
        pointerNode->setPosition(carNode->getPosition() + carNode->getOrientation() * pointerOffset);
        pointerNode->setOrientation(carNode->getOrientation() * pointerPitch * pointerRoll);

        // position steering wheel
        Ogre::Vector3 steeringWheelOffset(1.3, 2.5, 2.5);
        Ogre::Quaternion steeringWheelPitch(Ogre::Degree(15), Ogre::Vector3::UNIT_X);
        Ogre::Quaternion steeringWheelRoll(Ogre::Degree(keyboardSteer * -1.5 + UdpListener::steer * 450), Ogre::Vector3::UNIT_Z);
        steeringWheelNode->setOrientation(carNode->getOrientation() * steeringWheelPitch * steeringWheelRoll);
        steeringWheelNode->setPosition(carNode->getPosition() + carNode->getOrientation() * steeringWheelOffset);

		carNode->setVisible(false);
		cockpitNode->setVisible(true);
		pointerNode->setVisible(true);
		if(SHOW_STEERING_WHEEL)
			steeringWheelNode->setVisible(true);
		else
			steeringWheelNode->setVisible(false);
	}
	else if(cameraMode == THIRD_PERSON)
	{
		if(speed >= 0)
			cameraRotationOffset *= Ogre::Math::Pow(0.1, evt.timeSinceLastFrame);
		else
			cameraRotationOffset = cameraRotationOffset * Ogre::Math::Pow(0.1, evt.timeSinceLastFrame) + 180 * (1 - Ogre::Math::Pow(0.1, evt.timeSinceLastFrame));

		Ogre::Radian camAngle = carNode->getOrientation().getYaw() + Ogre::Degree(cameraRotationOffset);
		Ogre::Real camXOffset = -Ogre::Math::Sin(camAngle) * 25;
		Ogre::Real camYOffset = 8;
		Ogre::Real camZOffset = -Ogre::Math::Cos(camAngle) * 25;

		camera->setPosition(carNode->getPosition() + Ogre::Vector3(camXOffset, camYOffset, camZOffset));
		camera->lookAt(carNode->getPosition());

		carNode->setVisible(true);
		cockpitNode->setVisible(false);
		pointerNode->setVisible(false);
		steeringWheelNode->setVisible(false);
	}

    // debug output
	//std::cout << "Position: " << carNode->getPosition() << " Rotation: " << carNode->getOrientation().getYaw().valueDegrees() << std::endl;

	// if we reach this position of the code, no error has occurred
	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char** argv)
#endif
{
	// create application object
	DrivingSimulatorV1 app;
	try
	{
		// start application
		app.createCar();
		app.createScene1();
		app.start();
	}
	catch(Ogre::Exception e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
		#endif
	}

	return 0;
}



