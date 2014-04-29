BlitzPlus source release!

Workspace and project files are in MSVC 6.0 format.

Ide and Debugger require MFC which is included with MSVC 6.0, but not with later free/express versions of MSVC.

You can grab the prebuilt free version of blitzplus from www.blitzbasic.com.

Steps to build:

1) Install freeimage241 and fmodapi375win into same dir as blitzplus.

2) Build 'bblaunch', 'blitzcc', 'blitzplus', 'debugger', 'ide' and 'linker' projects in 'Win32 Release' config.

3) Build 'blitzplus' project in 'Win32 Debug' config.

4) Output files should end up in blitzplus/_release dir.

5) Also copy fmodapi375win/api/fmod.dll to blitzplus/_release/bin.

6) Done?
