@echo off
if exist x64 rd /s /q x64
if exist Win32 rd /s /q Win32

pushd
cd toroCLibrary
call clean.bat
popd

ping -n 2 127.0.0.0 > NUL
