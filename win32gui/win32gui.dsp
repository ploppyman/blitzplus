# Microsoft Developer Studio Project File - Name="win32gui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=win32gui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win32gui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32gui.mak" CFG="win32gui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "win32gui - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "win32gui - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "win32gui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /O1 /D "_LIB" /D "PLUS" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x1409 /d "NDEBUG"
# ADD RSC /l 0x1409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "win32gui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /Od /D "_LIB" /D "PLUS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x1409 /d "_DEBUG"
# ADD RSC /l 0x1409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "win32gui - Win32 Release"
# Name "win32gui - Win32 Debug"
# Begin Source File

SOURCE=.\win32button.cpp
# End Source File
# Begin Source File

SOURCE=.\win32button.h
# End Source File
# Begin Source File

SOURCE=.\win32combobox.cpp
# End Source File
# Begin Source File

SOURCE=.\win32combobox.h
# End Source File
# Begin Source File

SOURCE=.\win32desktop.cpp
# End Source File
# Begin Source File

SOURCE=.\win32desktop.h
# End Source File
# Begin Source File

SOURCE=.\win32gadget.cpp
# End Source File
# Begin Source File

SOURCE=.\win32gadget.h
# End Source File
# Begin Source File

SOURCE=.\win32gui.cpp
# End Source File
# Begin Source File

SOURCE=.\win32gui.h
# End Source File
# Begin Source File

SOURCE=.\win32htmlview.cpp
# End Source File
# Begin Source File

SOURCE=.\win32htmlview.h
# End Source File
# Begin Source File

SOURCE=.\win32iconstrip.cpp
# End Source File
# Begin Source File

SOURCE=.\win32iconstrip.h
# End Source File
# Begin Source File

SOURCE=.\win32label.cpp
# End Source File
# Begin Source File

SOURCE=.\win32label.h
# End Source File
# Begin Source File

SOURCE=.\win32listbox.cpp
# End Source File
# Begin Source File

SOURCE=.\win32listbox.h
# End Source File
# Begin Source File

SOURCE=.\win32menu.cpp
# End Source File
# Begin Source File

SOURCE=.\win32menu.h
# End Source File
# Begin Source File

SOURCE=.\win32panel.cpp
# End Source File
# Begin Source File

SOURCE=.\win32panel.h
# End Source File
# Begin Source File

SOURCE=.\win32progbar.cpp
# End Source File
# Begin Source File

SOURCE=.\win32progbar.h
# End Source File
# Begin Source File

SOURCE=.\win32slider.cpp
# End Source File
# Begin Source File

SOURCE=.\win32slider.h
# End Source File
# Begin Source File

SOURCE=.\win32splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\win32splitter.h
# End Source File
# Begin Source File

SOURCE=.\win32tabber.cpp
# End Source File
# Begin Source File

SOURCE=.\win32tabber.h
# End Source File
# Begin Source File

SOURCE=.\win32textarea.cpp
# End Source File
# Begin Source File

SOURCE=.\win32textarea.h
# End Source File
# Begin Source File

SOURCE=.\win32textfield.cpp
# End Source File
# Begin Source File

SOURCE=.\win32textfield.h
# End Source File
# Begin Source File

SOURCE=.\win32toolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\win32toolbar.h
# End Source File
# Begin Source File

SOURCE=.\win32treeview.cpp
# End Source File
# Begin Source File

SOURCE=.\win32treeview.h
# End Source File
# Begin Source File

SOURCE=.\win32window.cpp
# End Source File
# Begin Source File

SOURCE=.\win32window.h
# End Source File
# End Target
# End Project
