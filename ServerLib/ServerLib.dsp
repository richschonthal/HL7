# Microsoft Developer Studio Project File - Name="ServerLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ServerLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ServerLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerLib.mak" CFG="ServerLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ServerLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Live/Dev/SecurityClient/ServerLib", KPOFAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ServerLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ServerLib___Win32_SecurityServer_Debug"
# PROP BASE Intermediate_Dir "ServerLib___Win32_SecurityServer_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\Temp\Debug\ServerLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /D "_DEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FAs /Fa"C:\dev\ServerLib\\" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\stdobj" /D "_DEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"\dev\lib\ServerLibD.lib"
# ADD LIB32 /nologo /out:"..\lib\ServerLibD.lib"

!ELSEIF  "$(CFG)" == "ServerLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ServerLib___Win32_SecurityServer_Release"
# PROP BASE Intermediate_Dir "ServerLib___Win32_SecurityServer_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\Temp\Release\ServerLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\stdobj" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"\dev\lib\ServerLib.lib"
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ServerLib - Win32 Debug"
# Name "ServerLib - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Low Level"

# PROP Default_Filter ""
# Begin Group "Thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ThreadObject.cpp
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StateObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemControlObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemObject.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NTService.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\System.cpp
# End Source File
# End Group
# Begin Group "SubSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadPoolSubSystem.cpp
# End Source File
# End Group
# End Group
# Begin Group "High Level"

# PROP Default_Filter ""
# Begin Group "IO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IOSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\IOThread.cpp
# End Source File
# End Group
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSubSystem.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "ChangeNotification"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChangeNotificationContextObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeNotificationSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeNotificationThread.cpp
# End Source File
# Begin Source File

SOURCE=.\FileChangeNotificationContextObj.cpp
# End Source File
# Begin Source File

SOURCE=.\RegChangeNotificationContextObj.cpp
# End Source File
# End Group
# Begin Group "File Index"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileIndexDirObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIndexFileObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIndexObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIndexSubSystem.cpp
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Low Level Headers"

# PROP Default_Filter "h; hpp"
# Begin Source File

SOURCE=.\DBSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\IndexServiceNTService.h
# End Source File
# Begin Source File

SOURCE=.\IOSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\IOThread.h
# End Source File
# Begin Source File

SOURCE=.\LockObject.h
# End Source File
# Begin Source File

SOURCE=.\NTService.h
# End Source File
# Begin Source File

SOURCE=.\Parameter.h
# End Source File
# Begin Source File

SOURCE=.\StateObject.h
# End Source File
# Begin Source File

SOURCE=.\SubSystem.h
# End Source File
# Begin Source File

SOURCE=.\SubSystemControlThread.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfig.h
# End Source File
# Begin Source File

SOURCE=.\SystemControlObject.h
# End Source File
# Begin Source File

SOURCE=.\SystemObject.h
# End Source File
# Begin Source File

SOURCE=.\threadobject.h
# End Source File
# Begin Source File

SOURCE=.\ThreadObjectPool.h
# End Source File
# Begin Source File

SOURCE=.\ThreadObjectPoolControlThread.h
# End Source File
# End Group
# Begin Group "Misc Headers"

# PROP Default_Filter "h; hpp"
# Begin Source File

SOURCE=.\allocstring.h
# End Source File
# Begin Source File

SOURCE=.\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\CommonDefines.h
# End Source File
# Begin Source File

SOURCE=.\DBDatabase.h
# End Source File
# Begin Source File

SOURCE=.\DebugPrintf.h
# End Source File
# Begin Source File

SOURCE=.\PlatformInfo.h
# End Source File
# Begin Source File

SOURCE=.\QuickMail.h
# End Source File
# Begin Source File

SOURCE=.\ReadLock.h
# End Source File
# Begin Source File

SOURCE=.\ReadWriteObject.h
# End Source File
# Begin Source File

SOURCE=.\result.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WriteLock.h
# End Source File
# End Group
# Begin Group "Defines"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\WaitLen.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
