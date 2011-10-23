#include "Scene1.h"

Scene1::Scene1()
{

}

Scene1::~Scene1()
{

}

void Scene1::createScene()
{
	// load mini cooper
	Ogre::Entity* car = sceneManager->createEntity("MiniCooper.mesh");
	carNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	carNode->attachObject(car);
	carNode->scale(10, 10, 10);
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
		rotationSpeed += 300 * evt.timeSinceLastFrame;
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
		rotationSpeed -= 300 * evt.timeSinceLastFrame;

	// decrease rotation speed
	rotationSpeed *= Ogre::Math::Pow(0.4, evt.timeSinceLastFrame);

	// rotate car
	carNode->yaw(Ogre::Degree(rotationSpeed * evt.timeSinceLastFrame));

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



