#include "MainApplication.h"

#include <OgreException.h>

MainApplication::MainApplication()
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
	renderWindow = rootNode->initialise(true, "Driving Simulator - Test 3");

	// initialize resources
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create scene manager
	sceneManager = rootNode->createSceneManager("DefaultSceneManager");

	// create camera
	camera = sceneManager->createCamera("PlayerCam");
	camera->setAutoAspectRatio(true);
	camera->setNearClipDistance(1);

	// add viewport
	Ogre::Viewport* viewPort = renderWindow->addViewport(camera);
	viewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	// initialize OIS
	OIS::ParamList pList;
	size_t windowHandler = 0;
	std::ostringstream windowHandlerStr;
	renderWindow->getCustomAttribute("WINDOW", &windowHandler);
	windowHandlerStr << windowHandler;
	pList.insert(std::make_pair(std::string("WINDOW"), windowHandlerStr.str()));

	// prevent mouse pointer from disappearing under windows
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	pList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	#endif

	inputManager = OIS::InputManager::createInputSystem(pList);

	// create keyboard and mouse objects
	keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, false));
	mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, false));

    // create udp listener
    UdpListener::startUdpListener(DEFAULT_UDP_PORT_IN, DEFAULT_UDP_PORT_OUT, DEFAULT_REMOTE_ADDRESS);

	// initialize mouse clipping area by calling windowResized
	windowResized(renderWindow);

	// register as window event listener
	Ogre::WindowEventUtilities::addWindowEventListener(renderWindow, this);
}

MainApplication::~MainApplication()
{
    delete rootNode;
}

void MainApplication::start()
{
	// add frame listener and start render loop
	rootNode->addFrameListener(this);
	rootNode->startRendering();
}

// update mouse clipping area when window has been resized
void MainApplication::windowResized(Ogre::RenderWindow* window)
{
	unsigned int width, height, depth;
	int left, top;
	window->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& mouseState = mouse->getMouseState();
	mouseState.width = width;
	mouseState.height = height;
}

// destroy input objects and input system when window is closed
void MainApplication::windowClosed(Ogre::RenderWindow* window)
{
	if(window == renderWindow)
	{
		if(inputManager)
		{
			inputManager->destroyInputObject(keyboard);
			inputManager->destroyInputObject(mouse);
			OIS::InputManager::destroyInputSystem(inputManager);
			inputManager = NULL;
		}
	}
}


