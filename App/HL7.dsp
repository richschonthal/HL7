# Microsoft Developer Studio Project File - Name="HL7" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HL7 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HL7.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HL7.mak" CFG="HL7 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HL7 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "HL7 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HL7 - Win32 Release No SecServ" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Live/Dev/HL7_3", NYSDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HL7 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HL7___Win32_HL7_Debug"
# PROP BASE Intermediate_Dir "HL7___Win32_HL7_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Output\Debug"
# PROP Intermediate_Dir "..\Temp\Debug\HL7"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\SecurityServerLib" /I "..\stdobj" /I "..\ServerLib" /I "..\SecurityClient" /D "_NO_SECURITY_SERVER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DLL" /D "DB_SHARE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 stdobjD.lib ServerLibD.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ServerLibD.lib stdobjD.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "HL7 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HL7___Win32_HL7_Release"
# PROP BASE Intermediate_Dir "HL7___Win32_HL7_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Output\Release"
# PROP Intermediate_Dir "..\Temp\Release\HL7"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Z7 /O2 /I ".\\" /D "PCHTEST" /D "NDEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\SecurityServerLib" /I "..\stdobj" /I "..\ServerLib" /I "..\SecurityClient" /D "_NO_SECURITY_SERVER" /D "NDEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DB_SHARE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ServerLib.lib stdobj.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ServerLib.lib stdobj.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"..\lib\\"

!ELSEIF  "$(CFG)" == "HL7 - Win32 Release No SecServ"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HL7___Win32_HL7_Release_No_SecServ"
# PROP BASE Intermediate_Dir "HL7___Win32_HL7_Release_No_SecServ"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Output\Release"
# PROP Intermediate_Dir "..\Temp\Release\HL7noSec"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Z7 /O2 /I "SecurityServerLib" /I ".\\" /I "stdobj\include" /I "stdobj2\PACSDatabase" /I "stdobj2\StoredProcedure" /I "ServerLib" /D "NDEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DB_SHARE" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\SecurityServerLib" /I "..\stdobj" /I "..\ServerLib" /I "..\SecurityClient" /D "_NO_SECURITY_SERVER" /D "NDEBUG" /D "NO_DLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DB_SHARE" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ServerLib.lib stdobj.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"lib\release"
# ADD LINK32 ServerLib.lib stdobj.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\Output\Release/HL7NSS.exe" /libpath:"..\lib"

!ENDIF 

# Begin Target

# Name "HL7 - Win32 Debug"
# Name "HL7 - Win32 Release"
# Name "HL7 - Win32 Release No SecServ"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Monitor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\PACMonitor.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Group "Threads"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7PacOutThread.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputThreads.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerThreadInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadListAccess.cpp
# End Source File
# End Group
# Begin Group "Misc Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConnectionException.cpp
# End Source File
# Begin Source File

SOURCE=.\globals.cpp
# End Source File
# End Group
# Begin Group "IPC Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ProtocolObj.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\HL7NTService.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7System.cpp
# End Source File
# Begin Source File

SOURCE=.\Output.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Group "Def Lists"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7DBFieldDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBMessageDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBMsgSegDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBRuleDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBSegmentDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBVersionDef.cpp
# End Source File
# End Group
# Begin Group "Stored Procedure"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7DBMsgEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBNextMsgNum.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_GetProcessAction.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_RegisterProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_UnregisterProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBProcMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7SPNextMsgNum.cpp
# End Source File
# End Group
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7DBOutMsgFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBOutMSH.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBPACOut.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7DBServerConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7MessageConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7MSAConfig.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\HL7DBMsgSegFieldVal.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedDB.cpp
# End Source File
# End Group
# Begin Group "I/O Daemon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7FileDaemonThread.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemon.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemonInit.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemonThread.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7SocketDaemonThread.cpp
# End Source File
# End Group
# Begin Group "Message Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7DataTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7Message.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7MessageVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7Object.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7Segment.cpp
# End Source File
# End Group
# Begin Group "IO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HL7IOSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\HL7IOThread.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\HL7.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConnectionException.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\HL7.h
# End Source File
# Begin Source File

SOURCE=.\HL7DataTypes.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBMsgEvent.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBMsgSegFieldVal.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBNextMsgNum.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBOutMsgFormat.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBOutMSH.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBPACOut.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_GetProcessAction.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_RegisterProcess.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBPM_UnregisterProcess.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBProcMsg.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBRuleDef.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBSegmentDef.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBServerConfig.h
# End Source File
# Begin Source File

SOURCE=.\HL7DBVersionDef.h
# End Source File
# Begin Source File

SOURCE=.\HL7Field.h
# End Source File
# Begin Source File

SOURCE=.\HL7FileDaemonThread.h
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemon.h
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemonInit.h
# End Source File
# Begin Source File

SOURCE=.\HL7InputDaemonThread.h
# End Source File
# Begin Source File

SOURCE=.\HL7IOThread.h
# End Source File
# Begin Source File

SOURCE=.\HL7Message.h
# End Source File
# Begin Source File

SOURCE=.\HL7MessageConfig.h
# End Source File
# Begin Source File

SOURCE=.\HL7MessageVerify.h
# End Source File
# Begin Source File

SOURCE=.\HL7MSAConfig.h
# End Source File
# Begin Source File

SOURCE=.\HL7Object.h
# End Source File
# Begin Source File

SOURCE=.\HL7PacOutThread.h
# End Source File
# Begin Source File

SOURCE=.\HL7Segment.h
# End Source File
# Begin Source File

SOURCE=.\HL7SocketDaemonThread.h
# End Source File
# Begin Source File

SOURCE=.\HL7SPNextMsgNum.h
# End Source File
# Begin Source File

SOURCE=.\Monitor.h
# End Source File
# Begin Source File

SOURCE=.\Output.h
# End Source File
# Begin Source File

SOURCE=.\OutputConfig.h
# End Source File
# Begin Source File

SOURCE=.\PACMonitor.h
# End Source File
# Begin Source File

SOURCE=.\PACSDatabase.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolObj.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ServerThread.h
# End Source File
# Begin Source File

SOURCE=.\ServerThreadInfo.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemIPC.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\StatusOutThread.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StoredProcedure.h
# End Source File
# Begin Source File

SOURCE=.\ThreadListAccess.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\HL7.ico
# End Source File
# Begin Source File

SOURCE=.\res\HL7.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
