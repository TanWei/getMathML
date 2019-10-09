# Microsoft Developer Studio Project File - Name="XML" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XML - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XML.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XML.mak" CFG="XML - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XML - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XML - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/V-Fit/v12PKG_VdleCommon/Modules/XML", GAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XML - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Obj\XML\D"
# PROP Intermediate_Dir "..\..\..\Obj\XML\R"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W4 /GR /GX /O1 /I "..\..\..\v12PKG_FitVShell\Interface\XML" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\libR\XML.lib"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "..\..\..\Obj\XML\D"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\v12PKG_FitVShell\Interface\XML" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /FR /Yu"XML.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "XML - Win32 Release"
# Name "XML - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\XML.cpp
# ADD CPP /Yc"XML.h"
# End Source File
# Begin Source File

SOURCE=.\XMLAttribute.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLCDataSection.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLCharData.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLComment.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLDocFrag.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLDocType.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLElement.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLEntity.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLEntityRef.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLError.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLNode.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLNodeList.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLNodeMap.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLNotation.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLParseError.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLPI.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLText.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XMLTree.cpp

!IF  "$(CFG)" == "XML - Win32 Release"

# ADD CPP /Yu"XML.h"

!ELSEIF  "$(CFG)" == "XML - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\TFuncCalls.h
# End Source File
# End Group
# End Target
# End Project
