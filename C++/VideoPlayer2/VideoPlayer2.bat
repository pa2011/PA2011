@echo off

set MPLAYER_PATH="D:\MPlayer\mplayer.exe"
set VIDEO_PATH="D:\Videos\video2.mov"

cd %0\..\
..\Build\Windows\bin\Release\VideoPlayer2.exe %MPLAYER_PATH% %VIDEO_PATH% 5 150 100 52284 52285 "127.0.0.1"

