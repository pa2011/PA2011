#include "Scene1.h"

#define THIRD_PERSON 0
#define COCKPIT 1
#define ALLOW_REVERSE false

Scene1::Scene1()
{
    // initialize attributes
    speed = 0;
    cameraRotationOffset = 0;
    cameraMode = THIRD_PERSON;
    keyState[256] = {0};
}

Scene1::~Scene1()
{

}

void Scene1::createScene()
{
	// create world node
	worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	worldNode->scale(1, 1, 1);

	// load map
	Ogre::Entity* map = sceneManager->createEntity("Map1b.mesh");
	map->setCastShadows(false);
	worldNode->attachObject(map);

	// load car
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(4, 4, 4);
	carNode->setPosition(978, 0, 917);
	carNode->yaw(Ogre::Degree(-27));

	// load Cockpit
	Ogre::Entity* pointer = sceneManager->createEntity("pointer.mesh");
	pointerNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	pointerNode->attachObject(pointer);
	pointerNode->setPosition(0.00, 0.00, 0.00);
	pointerNode->scale(0.1, 0.1, 0.1);



	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(-1, -3, 2));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.3, 0.3, 0.3));

	// setup camera
	camera->setFOVy(Ogre::Degree(70));

	// enable shadow
	//sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	//sceneManager->setShadowFarDistance(500);
}

bool Scene1::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    // display fps
    //std::cout << 1/evt.timeSinceLastFrame << std::endl;

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
	speed *= Ogre::Math::Pow(0.7, evt.timeSinceLastFrame);

    // accelerate / brake car by keyboard input
	if(keyboard->isKeyDown(OIS::KC_UP))
	{
	    speed += 60 * evt.timeSinceLastFrame;
	}
	if(keyboard->isKeyDown(OIS::KC_DOWN))
	{
	    speed -= 90 * evt.timeSinceLastFrame;
	}

	// accelerate / brake car by udp input
	if(UdpListener::throttle >= 0)
	{
	    speed += UdpListener::throttle * 60 * evt.timeSinceLastFrame;
	}
	else
	{
	    speed += UdpListener::throttle * 90 * evt.timeSinceLastFrame;
	}

	if(!ALLOW_REVERSE)
	{
		if(speed < 0)
			speed = 0;
	}

	// calculate steer intensity
	Ogre::Real normalizedSpeed = Ogre::Math::Abs(speed / 180);
	Ogre::Real steerIntensity = 100 / (100 * (Ogre::Math::Pow(normalizedSpeed, 2)) + 1) * Ogre::Math::Pow(normalizedSpeed, 1.5);

	// rotate car by udp input
	carNode->yaw(Ogre::Degree(UdpListener::steer * -5 * evt.timeSinceLastFrame * speed));
    cameraRotationOffset += UdpListener::steer * 90 * evt.timeSinceLastFrame;

    // rotate car by keyboard input
    if(keyboard->isKeyDown(OIS::KC_LEFT) && speed != 0)
    {
        carNode->yaw(Ogre::Degree(steerIntensity * 90 * evt.timeSinceLastFrame * Ogre::Math::Abs(speed) / speed));
        cameraRotationOffset -= 45 * evt.timeSinceLastFrame * steerIntensity * Ogre::Math::Abs(speed) / speed;
    }
    if(keyboard->isKeyDown(OIS::KC_RIGHT) && speed != 0)
    {
        carNode->yaw(Ogre::Degree(steerIntensity * -90 * evt.timeSinceLastFrame * Ogre::Math::Abs(speed) / speed));
        cameraRotationOffset += 45 * evt.timeSinceLastFrame * steerIntensity * Ogre::Math::Abs(speed) / speed;
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
		Ogre::Radian camAngle = carNode->getOrientation().getYaw() - Ogre::Degree(20);
		Ogre::Real camXOffset = Ogre::Math::Cos(camAngle) * 1.3;
		Ogre::Real camYOffset = 4;
		Ogre::Real camZOffset = -Ogre::Math::Sin(camAngle) * 1.3;

		camera->setPosition(carNode->getPosition() + Ogre::Vector3(camXOffset, camYOffset, camZOffset));
		camera->setOrientation(carNode->getOrientation());
		camera->yaw(Ogre::Degree(180));

        pointerNode->setVisible(true);
		Ogre::Radian pointerAngle = carNode->getOrientation().getYaw() - Ogre::Degree(85);
		Ogre::Real pointerXOffset = Ogre::Math::Cos(pointerAngle) * 2.0;
		Ogre::Real pointerYOffset = 3;
		Ogre::Real pointerZOffset = -Ogre::Math::Sin(pointerAngle) * 2.0;

		pointerNode->setPosition(carNode->getPosition() + Ogre::Vector3(pointerXOffset, pointerYOffset, pointerZOffset));
		pointerNode->setOrientation(carNode->getOrientation());
		pointerNode->roll(Ogre::Degree(speed*0.8));
	}
	else if(cameraMode == THIRD_PERSON)
	{
		if(speed >= 0)
		{
			cameraRotationOffset *= Ogre::Math::Pow(0.1, evt.timeSinceLastFrame);
		}
		else
		{
			cameraRotationOffset = cameraRotationOffset * Ogre::Math::Pow(0.1, evt.timeSinceLastFrame) + 180 * (1 - Ogre::Math::Pow(0.1, evt.timeSinceLastFrame));
		}

		Ogre::Radian camAngle = carNode->getOrientation().getYaw() + Ogre::Degree(cameraRotationOffset);
		Ogre::Real camXOffset = -Ogre::Math::Sin(camAngle) * 25;
		Ogre::Real camYOffset = 8;
		Ogre::Real camZOffset = -Ogre::Math::Cos(camAngle) * 25;

		camera->setPosition(carNode->getPosition() + Ogre::Vector3(camXOffset, camYOffset, camZOffset));
		camera->lookAt(carNode->getPosition());

		pointerNode->setVisible(false);
	}

	// debug information
	//std::cout << "Position: " << carNode->getPosition() << " Rotation: " << carNode->getOrientation().getYaw().valueDegrees() << std::endl;
	//std::cout << "Speed: " << speed;
	//std::cout << " Steer Intensity: " << steerIntensity << std::endl;

	// if we reach this position of the code, there's no need to abort
	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char** argv)
#endif
{
	// create application object
	Scene1 app;
	try
	{
		// start application
		app.createScene();
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



