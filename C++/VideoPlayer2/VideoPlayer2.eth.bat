@echo off

set MPLAYER_PATH="D:\MPlayer\mplayer.exe"
set VIDEO_PATH="D:\Videos\video2.mov"

cd %0\..\
VideoPlayer2.bat %MPLAYER_PATH% %VIDEO_PATH%