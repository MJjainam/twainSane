# twainSane
This is PoC project on controlling scanners connected to Linux from Windows

There are two folders that you can see: Windows and Linux


On Windows side:
Build the the project on Windows(Using VS 2017) - You will get a AppInitHook.dll in debug folder on successful build.
Hook twaindsm.dll with this AppInitHook.dll

On Linux side:
use make command inside sane-backends
go inside sane-backends/frontend/twainBridge and run "./twainServer 2222"


On windows side open twacker application in 2.0 mode and start using it.


