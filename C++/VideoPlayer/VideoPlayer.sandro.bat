@echo off
cd %0\..\

set udp-port=52284
set vlc-path="C:\Program Files (x86)\VideoLAN\VLC\vlc.exe"
set vlc-port=4212
set vlc-password="ethz"
set channel-name="channel1"
set video-path="Resources\video1.mp4"

Build\Windows\Release\VideoPlayer.exe %udp-port% %vlc-path% %vlc-port% %vlc-password% %channel-name% %video-path%