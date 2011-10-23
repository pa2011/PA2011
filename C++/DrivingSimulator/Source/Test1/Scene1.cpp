#include "Scene1.h"

Scene1::Scene1()
{

}

Scene1::~Scene1()
{

}

void Scene1::createScene()
{
	// load ogre head
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(10, 10, 10);
	carNode->yaw(Ogre::Degree(-30));

	// position camera
	camera->setPosition(Ogre::Vector3(0, 10, 50));
	camera->lookAt(Ogre::Vector3(0, 5, 0));

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// create point light
	Ogre::Light* pointLight1 = sceneManager->createLight("PointLight1");
	pointLight1->setPosition(-100, 500, 1000);

	// initialize variables
	rotationSpeed = 0;
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

	// increase rotation speed (according to keyboard interaction)
	if(keyboard->isKeyDown(OIS::KC_LEFT))
		rotationSpeed += 3;
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
		rotationSpeed -= 3;

	// decrease rotation speed
	rotationSpeed *= 0.98;
	if(Ogre::Math::Abs(rotationSpeed) < 1)
		rotationSpeed = 0;

	// rotate car
	carNode->yaw(Ogre::Degree(rotationSpeed * evt.timeSinceLastFrame));

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



