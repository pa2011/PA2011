#include "Scene1.h"

Scene1::Scene1()
{

}

Scene1::~Scene1()
{

}

void Scene1::createScene()
{
	// load world
	Ogre::Entity* world = sceneManager->createEntity("World1.mesh");
	worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	worldNode->attachObject(world);
	worldNode->scale(1, 1, 1);

	// load mini cooper
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(5, 5, 5);
	carNode->yaw(Ogre::Degree(-30));

	// position camera
	camera->setPosition(Ogre::Vector3(0, 10, 50));
	camera->lookAt(Ogre::Vector3(0, 5, 0));

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

	// create point light
	Ogre::Light* sunLight = sceneManager->createLight("SunLight");
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(1, -1, -3));
	sunLight->setDiffuseColour(Ogre::ColourValue(0.9, 0.9, 0.9));
	sunLight->setSpecularColour(Ogre::ColourValue(0.9, 0.9, 0.9));

	// enable shadow
	sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

bool Scene1::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

	// increase speed (according to keyboard interaction)
	if(keyboard->isKeyDown(OIS::KC_UP))
		speed += 60 * evt.timeSinceLastFrame;
	if(keyboard->isKeyDown(OIS::KC_DOWN))
		speed -= 70 * evt.timeSinceLastFrame;


	// decrease speed
	speed *= Ogre::Math::Pow(0.5, evt.timeSinceLastFrame);

	// rotate car
	if(keyboard->isKeyDown(OIS::KC_LEFT))
		carNode->yaw(Ogre::Degree(2 * evt.timeSinceLastFrame * speed));
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
		carNode->yaw(Ogre::Degree(-2 * evt.timeSinceLastFrame * speed));

	// update car position
	Ogre::Real xMove = Ogre::Math::Sin(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	Ogre::Real zMove = Ogre::Math::Cos(carNode->getOrientation().getYaw()) * speed * evt.timeSinceLastFrame;
	carNode->translate(xMove, 0, zMove);

	// update camera
	Ogre::Real d = camera->getPosition().distance(carNode->getPosition());
	camera->setPosition(Ogre::Vector3(0, 10, 50));
	camera->setDirection(carNode->getPosition()-camera->getPosition());
	camera->moveRelative(Ogre::Vector3(0, 0, (20-d)*0.1));

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



