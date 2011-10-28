#include "Scene1.h"

Scene1::Scene1()
{
    // initialize attributes
    speed = 0;
    cameraRotationOffset = 0;
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
	Ogre::Entity* map = sceneManager->createEntity("Map1.mesh");
	map->setCastShadows(false);
	worldNode->attachObject(map);

	// load buildings
	Ogre::Entity* building = sceneManager->createEntity("Buildings.mesh");
	building->setCastShadows(true);
	worldNode->attachObject(building);

	// load car
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(4, 4, 4);
	carNode->setPosition(-5, 0, 0);

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(-1, -3, 2));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.3, 0.3, 0.3));

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

	// increase speed (according to keyboard interaction)
	if(keyboard->isKeyDown(OIS::KC_UP))
		speed += 60 * evt.timeSinceLastFrame;
	if(keyboard->isKeyDown(OIS::KC_DOWN))
		speed -= 70 * evt.timeSinceLastFrame;


	// decrease speed
	speed *= Ogre::Math::Pow(0.5, evt.timeSinceLastFrame);

	// rotate car
	if(keyboard->isKeyDown(OIS::KC_LEFT))
	{
		carNode->yaw(Ogre::Degree(2.5 * evt.timeSinceLastFrame * speed));
		cameraRotationOffset -= 45 * evt.timeSinceLastFrame;
	}
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
	{
		carNode->yaw(Ogre::Degree(-2.5 * evt.timeSinceLastFrame * speed));
		cameraRotationOffset += 45 * evt.timeSinceLastFrame;
	}

	cameraRotationOffset *= Ogre::Math::Pow(0.1, evt.timeSinceLastFrame);

	// update car position
	Ogre::Real xMove = Ogre::Math::Sin(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	Ogre::Real zMove = Ogre::Math::Cos(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	carNode->translate(xMove, 0, zMove);

	// update camera
	Ogre::Radian camAngle = carNode->getOrientation().getYaw() + Ogre::Degree(cameraRotationOffset);
    Ogre::Real camXOffset = -Ogre::Math::Sin(camAngle) * 35;
    Ogre::Real camYOffset = 12;
	Ogre::Real camZOffset = -Ogre::Math::Cos(camAngle) * 35;

	camera->setPosition(carNode->getPosition() + Ogre::Vector3(camXOffset, camYOffset, camZOffset));
    camera->lookAt(carNode->getPosition());

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



