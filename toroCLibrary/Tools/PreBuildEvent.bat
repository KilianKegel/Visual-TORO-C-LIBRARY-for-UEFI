@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################
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