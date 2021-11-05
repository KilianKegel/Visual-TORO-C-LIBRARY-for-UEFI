@echo off
for /F %%t in ('powershell get-date -f MMddHHmmssff') do set TIMESTAMP=%%t
if not exist $trash md $trash
attrib +h $trash


rem for /F %%f in ('dir /s /b /AD doxygen.tmp x64 Win32') do (
rem     md \$trash\%TIMESTAMP%\%%~pf
rem     move %%f \$trash\%TIMESTAMP%\%%~pf
rem )

for %%d in (x86 x64 Release) do (
    if not exist $trash\%TIMESTAMP%\%%~pd md $trash\%TIMESTAMP%\%%~pd
    if exist %%d move %%d $trash\%TIMESTAMP%\%%~pd
)

for %%f in (vc140.pdb) do (
        if not exist $trash\%TIMESTAMP%\%%~pf md $trash\%TIMESTAMP%\%%~pf
        if exist %%f move %%f $trash\%TIMESTAMP%\%%~pf
    )
)

cd tools
for %%f in (PreBuild.exe PreBuild.ilk PreBuild.pdb) do (
        if not exist $trash\%TIMESTAMP%\%%~pf md $trash\%TIMESTAMP%\%%~pf
        if exist %%f move %%f $trash\%TIMESTAMP%\%%~pf
    )
)
cd %~p0

rem if "%1" == "/all" (
rem     if exist ..\..\CdePkg\CdeLib\ia32\CdeLib.lib move ..\..\CdePkg\CdeLib\ia32\CdeLib.lib \$trash\%TIMESTAMP%
rem     if exist ..\..\CdePkg\CdeLib\x64\CdeLib.lib move ..\..\CdePkg\CdeLib\x64\CdeLib.lib \$trash\%TIMESTAMP%
rem     if exist ..\..\CdePkg\CdeServices\CdeServicesDxe32.efi move ..\..\CdePkg\CdeServices\CdeServicesDxe32.efi \$trash\%TIMESTAMP%
rem     if exist ..\..\CdePkg\CdeServices\CdeServicesDxe64.efi move ..\..\CdePkg\CdeServices\CdeServicesDxe64.efi \$trash\%TIMESTAMP%
rem     if exist ..\..\CdePkg\CdeServices\CdeServicesPei32.efi move ..\..\CdePkg\CdeServices\CdeServicesPei32.efi \$trash\%TIMESTAMP%
rem )