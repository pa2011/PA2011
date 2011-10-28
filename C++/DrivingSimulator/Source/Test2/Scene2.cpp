#include "Scene2.h"

Scene2::Scene2()
{

}

Scene2::~Scene2()
{

}

void Scene2::createScene()
{
	// create world node
	worldNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	worldNode->scale(0.1, 0.1, 0.1);

	// load simple geometry
	Ogre::Entity* plane = sceneManager->createEntity("SimplePlane.mesh");
	plane->setCastShadows(false);
	worldNode->attachObject(plane);
	Ogre::Entity* objects = sceneManager->createEntity("SimpleGeometry.mesh");
	worldNode->attachObject(objects);

	// create ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	// create sun light
	Ogre::Light* sunLight = sceneManager->createLight();
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(Ogre::Vector3(1, -0.8, 1));
	sunLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	sunLight->setSpecularColour(Ogre::ColourValue(0.7, 0.7, 0.7));

	// position camera
	camera->setPosition(-20, 20, 40);
	camera->lookAt(0, 0, 0);

	// enable shadow
	//sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	//sceneManager->setShadowFarDistance(500);
}

bool Scene2::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

	// update camera position
	if(keyboard->isKeyDown(OIS::KC_UP) && (camera->getPosition().y < 40))
		camera->moveRelative(Ogre::Vector3(0, 50*evt.timeSinceLastFrame, 0));
	if(keyboard->isKeyDown(OIS::KC_DOWN) && (camera->getPosition().y > 5))
		camera->moveRelative(Ogre::Vector3(0, -50*evt.timeSinceLastFrame, 0));
	if(keyboard->isKeyDown(OIS::KC_LEFT))
		camera->moveRelative(Ogre::Vector3(-50*evt.timeSinceLastFrame, 0, 0));
	if(keyboard->isKeyDown(OIS::KC_RIGHT))
		camera->moveRelative(Ogre::Vector3(50*evt.timeSinceLastFrame, 0, 0));

    camera->lookAt(Ogre::Vector3(0, 0, 0));

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
	Scene2 app;
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



