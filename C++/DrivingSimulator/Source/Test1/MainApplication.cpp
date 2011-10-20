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

bool MainApplication::go()
{
    // create root node
    #ifdef _DEBUG
    rootNode = new Ogre::Root("Resources/plugins_d.cfg", "Resources/ogre_d.cfg", "log_d.txt");
    #else
    rootNode = new Ogre::Root("Resources/plugins.cfg", "Resources/ogre.cfg", "log.txt");
    #endif

    // load config file
    Ogre::ConfigFile configFile;
    #ifdef _DEBUG
    configFile.load("Resources/resources_d.cfg");
    #else
    configFile.load("Resources/resources_d.cfg");
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

    // initialize window
    if(!(rootNode->restoreConfig()))
    {
        return false;
    }



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
			app.go();
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

