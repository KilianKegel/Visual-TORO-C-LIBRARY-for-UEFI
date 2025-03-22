@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################
echo ### PostBuild(CdeLib.lib) - start %date%%time%

if exist x64\*.obj del x64\*.obj

echo %~p0

if not exist  %~p0PostBuild.exe cl /nologo /Zi /Od %~p0PostBuild.c /Fe%~p0PostBuild.exe /Fo%~p0PostBuild.obj
if not exist  %~p0PostBuild.obj del %~p0PostBuild.obj


rem cmd /C tools\PostBuildEvent.bat $(SolutionDir) $(ProjectDir) $(Configuration) $(PlatformShortName)
rem echo 1.	%1
rem echo 2.	%2
rem echo 3.	%3
rem echo 4.	%4
rem echo 5.	%5
rem echo 6.	%6

REM
REM dublicate the library to LVMMUefiShell and LVVMWinNT  counterparts, before remove the unwanted object files
REM
echo copy /y %6 %~dp6\%~n6LLVMUefiShell.lib
copy /y %6 %~dp6\%~n6LLVMUefiShell.lib
echo copy /y %6 %~dp6\%~n6LLVMWinNT.lib
copy /y %6 %~dp6\%~n6LLVMWinNT.lib

echo %~p0PostBuild.exe %1 %2 %3 %4 %5 %6
%~p0PostBuild.exe %1 %2 %3 %4 %5 %6

echo lib /nologo @%2%5REMOVE4UEFISHELL%4.lst %~dp6%~n6LLVMUefiShell.lib
lib /nologo @%2%5REMOVE4UEFISHELL%4.lst %~dp6%~n6LLVMUefiShell.lib
echo lib /nologo @%2%5REMOVE4WINNT%4.lst %~dp6%~n6LLVMWinNT.lib
lib /nologo @%2%5REMOVE4WINNT%4.lst %~dp6%~n6LLVMWinNT.lib

:EOF
echo ### PostBuild(CdeLib.lib) - end   %date%%time%
:EXIT