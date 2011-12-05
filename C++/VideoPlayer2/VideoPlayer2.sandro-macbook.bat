@echo off

set MPLAYER_PATH="C:\Program Files (x86)\MPlayer\mplayer.exe"
set VIDEO_PATH="Z:\sandro\Movies\Autofahrt\tunnel.mov"

cd %0\..\
VideoPlayer2.bat %MPLAYER_PATH% %VIDEO_PATH% 5 150 100 52284 52285 "127.0.0.1"