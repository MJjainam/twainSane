# twainSane
This is PoC project on controlling scanners connected to Linux from Windows

There are two folders that you can see: Windows and Linux


On Windows side:
Build the the project on Windows(Using VS 2017) - You will get a AppInitHook.dll in debug folder on successful build.
Hook twaindsm.dll with this AppInitHook.dll

On Linux side:

1. First in sane-hook folder run `gcc saneHook.c -o saneHook.so -fPIC -shared -ldl`
This will hook the create a hooked library which will be hooked just before calling xsane
2. use `make` command inside sane-backends
go inside sane-backends/frontend/twainBridge and run `./twainServer 2222`


On Windows side:
1. Hook the twaindsm.dll - Can be done by AppInitHook 
 open twacker application in 2.0 mode and start using it.

#Ending the POC - What all is left?
    1. The implementation works well when the user selects the source and then gives aquire - direct aquire won't work. It won't take much time to implement that.
    2. The Image saved in the windows has some color change problem.
    


    