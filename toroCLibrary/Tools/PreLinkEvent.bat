@echo off
rem ######################################################################################
rem ### currently not used ###############################################################
rem ######################################################################################
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