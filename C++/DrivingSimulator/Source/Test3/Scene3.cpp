#include "Scene3.h"

Scene3::Scene3()
{
    // initialize attributes
    speed = 0;
    cameraRotationOffset = 0;
    keyState[256] = {0};
}

Scene3::~Scene3()
{

}

void Scene3::createScene()
{
	// create world node
	worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	worldNode->scale(0.1, 0.1, 0.1);

	// Scene 3
	Ogre::Entity* plane = sceneManager->createEntity("World2c.mesh");
	plane->setCastShadows(false);
	worldNode->attachObject(plane);

	// load car
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(4, 4, 4);
	carNode->setPosition(-5, 0, 0);

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(-0.2, -0.5, 0.5));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.7, 0.7, 0.7));

	// position camera
	//camera->setPosition(-20, 20, 40);
	//camera->lookAt(0, 0, 0);

	// enable shadow
	//sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	//sceneManager->setShadowFarDistance(500);
}

bool Scene3::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    // display fps
    //std::cout << 1/evt.timeSinceLastFrame << std::endl;

	// capture input devices
	if(inputManager)
	{
		keyboard->capture();
		mouse->capture();
	}


    // decrease speed (air resistance, etc...)
	speed *= Ogre::Math::Pow(0.5, evt.timeSinceLastFrame);

	// accelerate / brake car by keyboard input
	if(keyboard->isKeyDown(OIS::KC_UP))
	{
	    speed += 60 * evt.timeSinceLastFrame;
	}
	if(keyboard->isKeyDown(OIS::KC_DOWN))
	{
	    speed -= 90 * evt.timeSinceLastFrame;
	}

    // rotate car by keyboard input
    if(keyboard->isKeyDown(OIS::KC_LEFT))
    {
        carNode->yaw(Ogre::Degree(2.5 * evt.timeSinceLastFrame * speed));
        cameraRotationOffset -= 45 * evt.timeSinceLastFrame * Ogre::Math::Abs(speed) / speed;
    }
    if(keyboard->isKeyDown(OIS::KC_RIGHT))
    {
        carNode->yaw(Ogre::Degree(-2.5 * evt.timeSinceLastFrame * speed));
        cameraRotationOffset += 45 * evt.timeSinceLastFrame * Ogre::Math::Abs(speed) / speed;
    }


    // update car position
	Ogre::Real xMove = Ogre::Math::Sin(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	Ogre::Real zMove = Ogre::Math::Cos(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	carNode->translate(xMove, 0, zMove);


	// abort scene if window has been closed or escape button has been hit
	if(renderWindow->isClosed() || keyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
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

    /*if(speed < 0)
    {
        camXOffset = camXOffset * -1;
        camZOffset = camZOffset * -1;
    }*/
    camera->setPosition(carNode->getPosition() + Ogre::Vector3(camXOffset, camYOffset, camZOffset));
    camera->lookAt(carNode->getPosition());

	/*// update camera position
	if(keyboard->isKeyDown(OIS::KC_UP) && (camera->getPosition().y < 40))
		camera->moveRelative(Ogre::Vector3(0, 50*evt.timeSinceLastFrame, 0));
	if(keyboard->isKeyDown(OIS::KC_DOWN) && (camera->getPosition().y > 5))
		camera->moveRelative(Ogre::Vector3(0, -50*evt.timeSinceLastFrame, 0));
	if(keyboard->isKeyDown(OIS::KC_LEFT))
		camera->moveRelative(Ogre::Vector3(-50*evt.timeSinceLastFrame, 0, 0));
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
		camera->moveRelative(Ogre::Vector3(50*evt.timeSinceLastFrame, 0, 0));

    camera->lookAt(Ogre::Vector3(0, 0, 0));*/

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
	Scene3 app;
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



