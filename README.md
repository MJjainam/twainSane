Tested on Windows 10 64 bit and Debian stretch


# twainSane
This is PoC project on controlling scanners connected to Linux from Windows

There are two folders that you can see: Windows and Linux

Initial Setup:
    On Windows side:
    1. Open AppInitHook.sln
    2. Build the the project on Windows(Using VS 2017) 
        You will get a Debug/AppInitHook.dll on successful build.
    3. Copy this AppInitHook.dll to C:\ (You will need admin priviliges to do that - Google this you will get it easily)
    4. Open windows registery and go to `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Windows`
    5. Inside that change 1. AppInit_DLLs = C:\AppInitHook.dll
                        2. LoadAppInit_DLLs = 1
    6. Restart the system and your twaindsm.dll's DSM_Entry function is hooked                      


    On Linux side:

    1. First in sane-hook folder run `gcc saneHook.c -o saneHook.so -fPIC -shared -ldl`
    This will create a hooked library which will be hooked just before calling xsane
    2. use `make` command inside sane-backends




To Run:
    Linux side:
    1. Go inside sane-backends/frontend/twainBridge and run `./twainServer 2222`
    
    Windows side:
    1. Open twacker application and start using it (You need to select TWAIN 2.0 for this)




Open twacker application in 2.0 mode and start using it.

#Ending the POC - What all is left?
    1. The implementation works well when the user selects the source and then gives aquire - direct aquire won't work. It won't take much time to implement that.
    2. The Image saved in the windows has some color change problem.
    3. Many things in this project are hardcoded like the ip address of the systems I was using. I know it's bad but one can figure out those parts.


    


    