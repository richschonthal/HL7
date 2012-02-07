# Microsoft Developer Studio Project File - Name="ControlPanel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ControlPanel - Win32 HL7 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ControlPanel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ControlPanel.mak" CFG="ControlPanel - Win32 HL7 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ControlPanel - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlPanel - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlPanel - Win32 HL7 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlPanel - Win32 HL7 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Live/Dev/HL7_3", NYSDAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ControlPanel - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "NO_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 stdobj.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/HL7.cpl"

!ELSEIF  "$(CFG)" == "ControlPanel - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\app" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 stdobjD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/HL7.cpl" /pdbtype:sept /libpath:"..\lib"

!ELSEIF  "$(CFG)" == "ControlPanel - Win32 HL7 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ControlPanel___Win32_HL7_Debug"
# PROP BASE Intermediate_Dir "ControlPanel___Win32_HL7_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Output\Debug"
# PROP Intermediate_Dir "..\Temp\Debug\CP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\stdobj\include" /I ".\\" /I "..\\" /I "..\ServerLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 stdobjD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/HL7.cpl" /pdbtype:sept
# ADD LINK32 stdobjD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Output\Debug/HL7.cpl" /pdbtype:sept /libpath:"..\lib\debug"

!ELSEIF  "$(CFG)" == "ControlPanel - Win32 HL7 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ControlPanel___Win32_HL7_Release"
# PROP BASE Intermediate_Dir "ControlPanel___Win32_HL7_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Output\Release"
# PROP Intermediate_Dir "..\Temp\Release\CP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "NO_DLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\stdobj\include" /I ".\\" /I "..\\" /I "..\ServerLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "NO_DLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 stdobj.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/HL7.cpl"
# ADD LINK32 stdobj.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\Output\Release/HL7.cpl" /libpath:"..\lib\release"

!ENDIF 

# Begin Target

# Name "ControlPanel - Win32 Release"
# Name "ControlPanel - Win32 Debug"
# Name "ControlPanel - Win32 HL7 Debug"
# Name "ControlPanel - Win32 HL7 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChooseFileBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPanel.def
# End Source File
# Begin Source File

SOURCE=.\ControlPanel.rc
# End Source File
# Begin Source File

SOURCE=..\App\HL7InputDaemonInit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainPropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\App\OutputConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\PropFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageInput.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageMain.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageSecurity.cpp
# End Source File
# Begin Source File

SOURCE=.\PropSockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectNewModuleTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ControlPanel.h
# End Source File
# Begin Source File

SOURCE=.\PropPageDebug.h
# End Source File
# Begin Source File

SOURCE=.\PropPageSecurity.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\x.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ControlPanel.rc2
# End Source File
# Begin Source File

SOURCE=.\res\HL7.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
