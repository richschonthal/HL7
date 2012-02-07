# Microsoft Developer Studio Project File - Name="stdobj" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=stdobj - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "stdobj.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "stdobj.mak" CFG="stdobj - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "stdobj - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "stdobj - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "stdobj - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "stdobj - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/stdobj", LSCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "stdobj - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\dev\lib\stdobj.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy \dev\lib\stdobj.lib f:\Lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "stdobj - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"\dev\lib\Debug/stdobj.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\dev\lib\stdobjD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy \dev\lib\stdobjD.lib f:\Lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "stdobj___Win32_Static_Release"
# PROP BASE Intermediate_Dir "stdobj___Win32_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "stdobj___Win32_Static_Release"
# PROP Intermediate_Dir "stdobj___Win32_Static_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"\dev\lib\stdobj.lib"
# ADD LIB32 /nologo /out:"\dev\lib\stdobjstat.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy \dev\lib\stdobjstat.lib f:\Lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "stdobj___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "stdobj___Win32_Static_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "stdobj___Win32_Static_Debug"
# PROP Intermediate_Dir "stdobj___Win32_Static_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"\dev\lib\Debug/stdobj.bsc"
# ADD BSC32 /nologo /o"\dev\lib\Debug/stdobj.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"\dev\lib\stdobjD.lib"
# ADD LIB32 /nologo /out:"\dev\lib\stdobjstatD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy \dev\lib\stdobjstatD.lib f:\Lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "stdobj - Win32 Release"
# Name "stdobj - Win32 Debug"
# Name "stdobj - Win32 Static Release"
# Name "stdobj - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Registry Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Registry\regkey.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry\regkeyinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry\regresult.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry\regvalue.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry\regvaluemultisz.cpp
# End Source File
# End Group
# Begin Group "Socket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LockedSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallSocketFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TextSocket.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLine.cpp

!IF  "$(CFG)" == "stdobj - Win32 Release"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Debug"

# SUBTRACT CPP /I "..\\"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Release"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Debug"

# SUBTRACT BASE CPP /I "..\\"
# SUBTRACT CPP /I "..\\"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugPrintf.cpp
# End Source File
# Begin Source File

SOURCE=.\DelimFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgItemGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecuteApp.cpp
# End Source File
# Begin Source File

SOURCE=.\folder_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle.cpp
# End Source File
# Begin Source File

SOURCE=.\IPAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadString.cpp
# End Source File
# Begin Source File

SOURCE=.\LockedResource.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\NamedPipeFile.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\PathString.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Status.cpp

!IF  "$(CFG)" == "stdobj - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StatusOutThread.cpp

!IF  "$(CFG)" == "stdobj - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "stdobj - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Debug"

# ADD CPP /Yc"stdafx.h"
# SUBTRACT CPP /I "..\\"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "stdobj - Win32 Static Debug"

# ADD BASE CPP /Yc"stdafx.h"
# SUBTRACT BASE CPP /I "..\\"
# ADD CPP /Yc"stdafx.h"
# SUBTRACT CPP /I "..\\"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=.\WordWrap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=F:\include\bit.h
# End Source File
# Begin Source File

SOURCE=..\include\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=..\include\CmdLine.h
# End Source File
# Begin Source File

SOURCE=..\include\DebugPrintf.h
# End Source File
# Begin Source File

SOURCE=F:\include\DlgItemGroup.h
# End Source File
# Begin Source File

SOURCE=..\include\ExecuteApp.h
# End Source File
# Begin Source File

SOURCE=..\include\folder_dialog.h
# End Source File
# Begin Source File

SOURCE=.\Handle.h
# End Source File
# Begin Source File

SOURCE=.\IPAddress.h
# End Source File
# Begin Source File

SOURCE=.\LoadString.h
# End Source File
# Begin Source File

SOURCE=.\LockedSocket.h
# End Source File
# Begin Source File

SOURCE=.\MiniEncrypt.h
# End Source File
# Begin Source File

SOURCE=..\include\PathString.h
# End Source File
# Begin Source File

SOURCE=..\include\ProcessInfo.h
# End Source File
# Begin Source File

SOURCE=..\include\registry.h
# End Source File
# Begin Source File

SOURCE=J:\include\ServiceStatus.h
# End Source File
# Begin Source File

SOURCE=..\include\SettingsFile.h
# End Source File
# Begin Source File

SOURCE=.\SmallSocketFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextSocket.h
# End Source File
# Begin Source File

SOURCE=F:\include\zProcess.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
