@echo on

set MPLAYER_PATH="C:\Program Files (x86)\MPlayer\mplayer.exe"
set VIDEO_PATH="C:\Users\Sandro Ropelato\Videos\Autofahrt\tunnel.mov"

cd %0\..\
VideoPlayer2.bat %MPLAYER_PATH% %VIDEO_PATH% 5 150 100 52284 52285 "127.0.0.1"