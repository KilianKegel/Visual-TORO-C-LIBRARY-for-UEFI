@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################
set ONE=1
for /F "tokens=1-3" %%a in ('type %~p0..\Include\BUILDNUM.h') do set BUILDOLD=%%c

set /a "BUILD=%BUILDOLD%+%ONE%"
echo #define BUILDNUM %BUILD% > %~p0..\Include\BUILDNUM.h
type %~p0..\Include\BUILDNUM.h

set BUILDOLD=
set BUILD=

goto EOF
echo ### PreBuild - start %date%%time%
echo %~p0
rem if not exist %~p0PreBuild.exe (
	del %~p0PreBuild.exe
	cl /nologo /Zi /Od %~p0PreBuild.c /Fe%~p0PreBuild.exe /Fo%~p0PreBuild.obj
	echo del %~p0PreBuild.obj
    del %~p0PreBuild.obj
rem )
echo %~p0PreBuild.exe %1 %2 %3 %~x1
%~p0PreBuild.exe %1 %2 %3 %~x1
echo ### PreBuild - end   %date%%time%
:EOF