# Microsoft Developer Studio Project File - Name="pha" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=pha - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pha.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pha.mak" CFG="pha - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pha - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pha - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pha - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "WIN_DOS_HACK" /FR /YX /FD /c
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "pha - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "WIN_DOS_HACK" /YX /FD /GZ /c
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pha - Win32 Release"
# Name "pha - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\chyba.c
# End Source File
# Begin Source File

SOURCE=..\cotyvar.c
# End Source File
# Begin Source File

SOURCE=..\cteni.c
# End Source File
# Begin Source File

SOURCE=..\datastr.c
# End Source File
# Begin Source File

SOURCE=..\dejtah.c
# End Source File
# Begin Source File

SOURCE=..\dosio.c
# End Source File
# Begin Source File

SOURCE=..\generato.c
# End Source File
# Begin Source File

SOURCE=..\globruti.c
# End Source File
# Begin Source File

SOURCE=..\hash.c
# End Source File
# Begin Source File

SOURCE=..\hashpech.c
# End Source File
# Begin Source File

SOURCE=..\hashtab.c
# End Source File
# Begin Source File

SOURCE=..\knihovna.c
# End Source File
# Begin Source File

SOURCE=..\kontrola.c
# End Source File
# Begin Source File

SOURCE=..\lokruti.c
# End Source File
# Begin Source File

SOURCE=..\myslitel.c
# End Source File
# Begin Source File

SOURCE=..\obk_ch.c
# End Source File
# Begin Source File

SOURCE=..\ohodnoc.c
# End Source File
# Begin Source File

SOURCE=..\partie.c
# End Source File
# Begin Source File

SOURCE=..\pgn.c
# End Source File
# Begin Source File

SOURCE=..\sachy.c
# End Source File
# Begin Source File

SOURCE=..\scio.c
# End Source File
# Begin Source File

SOURCE=..\soubvv.c
# End Source File
# Begin Source File

SOURCE=..\strtah.c
# End Source File
# Begin Source File

SOURCE=..\tabulky.c
# End Source File
# Begin Source File

SOURCE=..\ticho.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\chyba.h
# End Source File
# Begin Source File

SOURCE=..\cotyvar.h
# End Source File
# Begin Source File

SOURCE=..\cteni.h
# End Source File
# Begin Source File

SOURCE=..\datastr.h
# End Source File
# Begin Source File

SOURCE=..\dejtah.h
# End Source File
# Begin Source File

SOURCE=..\dosio.h
# End Source File
# Begin Source File

SOURCE=..\generato.h
# End Source File
# Begin Source File

SOURCE=..\globruti.h
# End Source File
# Begin Source File

SOURCE=..\hash.h
# End Source File
# Begin Source File

SOURCE=..\hashpech.h
# End Source File
# Begin Source File

SOURCE=..\hashtab.h
# End Source File
# Begin Source File

SOURCE=..\knihovna.h
# End Source File
# Begin Source File

SOURCE=..\kontrola.h
# End Source File
# Begin Source File

SOURCE=..\lokruti.h
# End Source File
# Begin Source File

SOURCE=..\myslitel.h
# End Source File
# Begin Source File

SOURCE=..\obk_ch.h
# End Source File
# Begin Source File

SOURCE=..\ohodnoc.h
# End Source File
# Begin Source File

SOURCE=..\partiar.h
# End Source File
# Begin Source File

SOURCE=..\partie.h
# End Source File
# Begin Source File

SOURCE=..\pgn.h
# End Source File
# Begin Source File

SOURCE=..\pole.h
# End Source File
# Begin Source File

SOURCE=..\promdlg.h
# End Source File
# Begin Source File

SOURCE=..\sachy.h
# End Source File
# Begin Source File

SOURCE=..\scio.h
# End Source File
# Begin Source File

SOURCE=..\soubvv.h
# End Source File
# Begin Source File

SOURCE=..\strtah.h
# End Source File
# Begin Source File

SOURCE=..\tabulky.h
# End Source File
# Begin Source File

SOURCE=..\ticho.h
# End Source File
# Begin Source File

SOURCE=..\volby.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
