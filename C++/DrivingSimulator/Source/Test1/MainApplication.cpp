#include "MainApplication.h"

#include <OgreException.h>

MainApplication::MainApplication()
{
    // constructor
}

MainApplication::~MainApplication()
{
    delete rootNode;
}

void MainApplication::init()
{
    // create root node
    #ifdef _DEBUG
    rootNode = new Ogre::Root("Resources/plugins_d.cfg", "Resources/graphics_d.cfg", "log_d.txt");
    #else
    rootNode = new Ogre::Root("Resources/plugins.cfg", "Resources/graphics.cfg", "log.txt");
    #endif

    // load config file
    Ogre::ConfigFile configFile;
    #ifdef _DEBUG
    configFile.load("Resources/resources_d.cfg");
    #else
    configFile.load("Resources/resources.cfg");
    #endif

    // load resource files
    Ogre::ConfigFile::SectionIterator it = configFile.getSectionIterator();
    while(it.hasMoreElements())
    {
        Ogre::String sectionName = it.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = it.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i = settings->begin(); i != settings->end(); ++i)
        {
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(i->second, i->first, sectionName);
        }
    }

    // try to initialize window with settings from config file
    if(!(rootNode->restoreConfig()))
    {
    	throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Could not read graphics configuration", "MainApplication::go");
    }
	renderWindow = rootNode->initialise(true, "Driving Simulator - Test 1");

	// initialize resources
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create scene manager
	sceneManager = rootNode->createSceneManager("DefaultSceneManager");

	// create camera
	camera = sceneManager->createCamera("PlayerCam");
	camera->setAutoAspectRatio(true);
	camera->setNearClipDistance(5);

	// add viewport
	Ogre::Viewport* viewPort = renderWindow->addViewport(camera);
	viewPort->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
}

void MainApplication::createScene()
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
}

void MainApplication::start()
{
	// add frame listener and start render loop
	rootNode->addFrameListener(this);
	rootNode->startRendering();
}

bool MainApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(renderWindow->isClosed())
		return false;

	carNode->yaw(Ogre::Degree(30 * evt.timeSinceLastFrame));

	return true;
}

#ifdef __cplusplus
extern "C"
{
#endif

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
	#else
	int main(int argc, char** argv)
	#endif
	{
		// create application object
		MainApplication app;
		try
		{
			// start application
			app.init();
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

#ifdef __cplusplus
}
#endif

