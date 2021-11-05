@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################
goto EXIT
echo ### PostBuild(CdeLib.lib) - start %date%%time%

goto EOF

if exist x64\*.obj del x64\*.obj

echo %~p0

del %~p0PostBuild.exe
cl /nologo /Zi /Od %~p0PostBuild.c /Fe%~p0PostBuild.exe /Fo%~p0PostBuild.obj
echo del %~p0PostBuild.obj
del %~p0PostBuild.obj

echo %~p0PostBuild.exe %1 %2 %3 %~x1

echo re-create the library...

rem cmd /C tools\PostBuildEvent.bat $(SolutionDir) $(ProjectDir) $(Configuration) $(PlatformShortName)
%~p0PostBuild.exe %1 %2 %3 %~x1 %4

echo finished

pushd
cd %4
dir /b mod*.obj > modules.lst

if "%4" == "x64" (
    lib /nologo @modules.lst /out:%1\CdePkg\CdeLib\x64\CdeLib.lib
) else (
    lib /nologo @modules.lst /out:%1\CdePkg\CdeLib\ia32\CdeLib.lib
)

rem if exist *.obj del *.obj

popd

:EOF
echo ### PostBuild(CdeLib.lib) - end   %date%%time%
:EXIT