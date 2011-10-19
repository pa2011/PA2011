#include "MainApplication.h"

#include <OgreException.h>

MainApplication::MainApplication()
{
    //ctor
}

MainApplication::~MainApplication()
{
    //dtor
}

bool MainApplication::go()
{
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

