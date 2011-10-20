Building DrivingSimulator on Windows
====================================

This setup has been made for CodeBlocks 10.05 [1] with MinGW 4.6.1 [2].
It uses Ogre3D SDK version 1.7.2 for MinGW [3].
The included makefile(s) use the MSys version of commands like find, etc. so make sure your MinGW and MSys bin paths appears before the system default commands in your PATH environment variable.


Environment variables
---------------------

The following environment variables need to be set:

OGRE_HOME: pointing to your OgreSDK folder (use forward instead of back slashes) (e.g. "C:/SDKs/OgreSDK")
MINGW_HOME: pointing to your MinGW folder (e.g. "C:/MinGW")


Downloads
---------

[1] http://sourceforge.net/projects/codeblocks/files/Binaries/10.05/Windows/codeblocks-10.05-setup.exe

[2] http://sourceforge.net/projects/mingw/files/Automated%20MinGW%20Installer/mingw-get-inst/mingw-get-inst-20110802/mingw-get-inst-20110802.exe/download?source=files

[3] http://sourceforge.net/projects/ogre/files/ogre/1.7/OgreSDK_mingw_v1-7-2.exe/download