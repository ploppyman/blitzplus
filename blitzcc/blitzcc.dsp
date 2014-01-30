# Microsoft Developer Studio Project File - Name="blitzcc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=blitzcc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blitzcc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blitzcc.mak" CFG="blitzcc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blitzcc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "blitzcc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blitzcc - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /GX /O1 /D "_CONSOLE" /D "PLUS" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x1409 /d "NDEBUG"
# ADD RSC /l 0x1409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"../_release/bin/blitzcc.exe"

!ELSEIF  "$(CFG)" == "blitzcc - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GR /GX /Od /D "_CONSOLE" /D "PLUS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FD /c
# ADD BASE RSC /l 0x1409 /d "_DEBUG"
# ADD RSC /l 0x1409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"../../release/Blitzplusrelease/bin/blitzcc.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "blitzcc - Win32 Release"
# Name "blitzcc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\assem_x86\asm_insts.cpp
# End Source File
# Begin Source File

SOURCE=.\assem_x86\assem_x86.cpp
# End Source File
# Begin Source File

SOURCE=.\codegen_x86\codegen_x86.cpp
# End Source File
# Begin Source File

SOURCE=.\decl.cpp
# End Source File
# Begin Source File

SOURCE=.\declnode.cpp
# End Source File
# Begin Source File

SOURCE=.\environ.cpp
# End Source File
# Begin Source File

SOURCE=.\exprnode.cpp
# End Source File
# Begin Source File

SOURCE=.\libs.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\node.cpp
# End Source File
# Begin Source File

SOURCE=.\assem_x86\operand.cpp
# End Source File
# Begin Source File

SOURCE=.\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\prognode.cpp
# End Source File
# Begin Source File

SOURCE=.\std.cpp
# End Source File
# Begin Source File

SOURCE=.\stdutil.cpp
# End Source File
# Begin Source File

SOURCE=.\stmtnode.cpp
# End Source File
# Begin Source File

SOURCE=.\codegen_x86\tile.cpp
# End Source File
# Begin Source File

SOURCE=.\toker.cpp
# End Source File
# Begin Source File

SOURCE=.\type.cpp
# End Source File
# Begin Source File

SOURCE=.\varnode.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\assem.h
# End Source File
# Begin Source File

SOURCE=.\assem_x86\assem_x86.h
# End Source File
# Begin Source File

SOURCE=.\codegen.h
# End Source File
# Begin Source File

SOURCE=.\codegen_x86\codegen_x86.h
# End Source File
# Begin Source File

SOURCE=.\decl.h
# End Source File
# Begin Source File

SOURCE=.\declnode.h
# End Source File
# Begin Source File

SOURCE=.\environ.h
# End Source File
# Begin Source File

SOURCE=.\ex.h
# End Source File
# Begin Source File

SOURCE=.\exprnode.h
# End Source File
# Begin Source File

SOURCE=.\assem_x86\insts.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\libs.h
# End Source File
# Begin Source File

SOURCE=.\linker.H
# End Source File
# Begin Source File

SOURCE=.\node.h
# End Source File
# Begin Source File

SOURCE=.\nodes.h
# End Source File
# Begin Source File

SOURCE=.\assem_x86\operand.h
# End Source File
# Begin Source File

SOURCE=.\parser.h
# End Source File
# Begin Source File

SOURCE=.\prognode.h
# End Source File
# Begin Source File

SOURCE=.\runtime.h
# End Source File
# Begin Source File

SOURCE=.\std.h
# End Source File
# Begin Source File

SOURCE=.\stdutil.h
# End Source File
# Begin Source File

SOURCE=.\stmtnode.h
# End Source File
# Begin Source File

SOURCE=.\codegen_x86\tile.h
# End Source File
# Begin Source File

SOURCE=.\toker.h
# End Source File
# Begin Source File

SOURCE=.\type.h
# End Source File
# Begin Source File

SOURCE=.\varnode.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\config.h
# End Source File
# End Target
# End Project
