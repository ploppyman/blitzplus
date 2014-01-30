# Microsoft Developer Studio Project File - Name="blitzplus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=blitzplus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blitzplus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blitzplus.mak" CFG="blitzplus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blitzplus - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blitzplus - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blitzplus - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLITZPLUS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /O1 /D "_WINDOWS" /D "_USRDLL" /D "BLITZPLUS_EXPORTS" /D "PLUS" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1409 /d "NDEBUG"
# ADD RSC /l 0x1409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 amstrmid.lib dxguid.lib opengl32.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386 /force /out:"../../release/blitzplusrelease\bin\runtime.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "blitzplus - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLITZPLUS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /Od /D "_WINDOWS" /D "_USRDLL" /D "BLITZPLUS_EXPORTS" /D "PLUS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1409 /d "_DEBUG"
# ADD RSC /l 0x1409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 amstrmid.lib dxguid.lib opengl32.lib wsock32.lib comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386 /force /out:"../../release/blitzplusrelease/bin/runtime_dbg.dll"
# SUBTRACT LINK32 /map /debug

!ENDIF 

# Begin Target

# Name "blitzplus - Win32 Release"
# Name "blitzplus - Win32 Debug"
# Begin Source File

SOURCE=.\b2d_link.cpp
# End Source File
# Begin Source File

SOURCE=.\basic.cpp

!IF  "$(CFG)" == "blitzplus - Win32 Release"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "blitzplus - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\basic.h
# End Source File
# Begin Source File

SOURCE=.\blitzplus.rc
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\gui_link.cpp
# End Source File
# Begin Source File

SOURCE=.\link.cpp
# End Source File
# Begin Source File

SOURCE=.\link.h
# End Source File
# Begin Source File

SOURCE=.\runtime.h
# End Source File
# Begin Source File

SOURCE=.\sys_link.cpp
# End Source File
# Begin Source File

SOURCE=.\userlibs.cpp
# End Source File
# Begin Source File

SOURCE=.\userlibs.h
# End Source File
# Begin Source File

SOURCE=..\..\FreeImage241\Source\FreeImageLib\Release\FreeImage.lib
# End Source File
# Begin Source File

SOURCE=..\..\fmodsrc375win\win\Final\fmodstatic.lib
# End Source File
# End Target
# End Project
