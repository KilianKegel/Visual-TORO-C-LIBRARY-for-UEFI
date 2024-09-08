@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################

REM debug 	echo 0: %0
REM debug 	echo 1: %1
REM debug 	echo 2. %2
REM debug 	echo 3. %3
REM debug 	echo 4. %4
REM debug 	echo 5. %5
REM debug 	echo 6. %6

if exist %1MSFTINTRIN.lib goto SKIP_INTRINLIB
if "%4" == "x64" goto SKIP_INTRINLIB

set path=%path%;%LIB%
for /F "delims=;" %%a in ('where libcmt.lib') do (

	@echo retrieving  intrinsic modules from "%%a"

	lib /nologo /list "%%a" | find "\ll"         > intrin.lst
	lib /nologo /list "%%a" | find "\ull"       >> intrin.lst
	lib /nologo /list "%%a" | find "\ftol3.obj" >> intrin.lst

	if exist %1\OBJExtractScript.txt del %1\OBJExtractScript.txt
	for /F "delims=" %%b in ('type intrin.lst') do (
		echo ^    ^%%~nxb
		REM echo /EXTRACT:%%b /OUT:%~dpnx1%%~nxb >> %1\OBJExtractScript.txt
		echo /EXTRACT:%%b /OUT:%1%%~nxb >> %1\OBJExtractScript.txt
	)
	echo lib /nologo "%%a" @%1OBJExtractScript.txt
	rem lib /nologo "%%a" @%1OBJExtractScript.txt

	for /F "delims=" %%l in (%1OBJExtractScript.txt) do (
		echo lib /NOLOGO "%%a" %%l
		lib /NOLOGO "%%a" %%l
	)
	lib /nologo /OUT:%1MSFTINTRIN.lib %1ll*.obj %1ull*.obj %1ftol3.obj 
)
:SKIP_INTRINLIB

goto EOF
echo ### PreLink - start %date%%time%
rem echo %~p0 %1 
pushd
rem change to the $(IntDir)
cd %1
dir /b /AA *.obj > object.lst
editbin /nologo /SECTION:.xdata,m /SECTION:.pdata,m @object.lst | find /V "LNK4039"
attrib -A *.obj
popd
echo ### PreLink - end   %date%%time%
:EOF