@echo off
rem ######################################################################################
rem ### get intrinsics from default Microsoft library ####################################
rem ######################################################################################
goto EXIT
echo ### PostBuild(CdeLib.lib) - start %date%%time%

if exist x64\*.obj del x64\*.obj

echo %~p0

if not exist  %~p0PostBuild.exe cl /nologo /Zi /Od %~p0PostBuild.c /Fe%~p0PostBuild.exe /Fo%~p0PostBuild.obj
if not exist  %~p0PostBuild.obj del %~p0PostBuild.obj

echo %~p0PostBuild.exe %1 %2 %3 %~x1 %4 %5 %6

echo re-create the library...

rem cmd /C tools\PostBuildEvent.bat $(SolutionDir) $(ProjectDir) $(Configuration) $(PlatformShortName)
%~p0PostBuild.exe %1 %2 %3 %~x1 %4 %5 %6

:EOF
echo ### PostBuild(CdeLib.lib) - end   %date%%time%
:EXIT