BlitzPlus source release!

Workspace and project files are in MSVC 6.0 format.

Ide and Debugger require MFC which is included with MSVC 6.0, but not with later free/express versions of MSVC.

You can grab the prebuilt free version of blitzplus from www.blitzbasic.com.

Steps to build:

1) Install freeimage241 into same dir as blitzplus: http://monkeycoder.co.nz/downloads/freeimage241.zip

2) Install fmodapi375 into same dir as blitzplus: http://www.fmod.org/files/public/fmodapi375win.zip

3) Open blitzplus workspace in MSVC 6.0.
 
4) Build 'bblaunch', 'blitzcc', 'blitzplus', 'debugger', 'ide' and 'linker' projects in 'Win32 Release' config.

5) Build 'blitzplus' project in 'Win32 Debug' config.

6) Output files should end up in blitzplus/_release dir.

7) Also copy fmodapi375win/api/fmod.dll to blitzplus/_release/bin.

8) Done?
