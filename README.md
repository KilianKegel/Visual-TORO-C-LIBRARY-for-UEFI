![Visitor](https://visitor-badge.laobi.icu/badge?page_id=KilianKegel.kiliankegel)
# torosrc

[**toro C Library**](https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library)

```
    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0
```

## Revision history

### 20230909
* add `CDE`(*C Development Environment*)-interface for native **Tianocore UEFI SHELL** [`UEFISHELLDRV`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/tree/main/toroCLibrary/OSInterface/UEFISHELLDRV).
  Enable [**Toro-C-Library**–`DXE`](https://github.com/KilianKegel/toro-C-Library#implementation-status) function set at `CRT0()`
  and full [**Toro-C-Library**–`SHELL`](https://github.com/KilianKegel/toro-C-Library#implementation-status) function set with availability
  of the `EfiShellProtocol`/`EFI_SHELL_PROTOCOL_GUID`:<br>
  https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/tree/main/toroCLibrary/OSInterface/UEFISHELLDRV<br>

  This is the foundation of an **ANSI-C-API**-extended **CDE UEFI SHELL**

* prepare `system()` **ANSI-C-API** call for MSDOS drive name support (**A:**, **B:**, **C:** ...) coming soon for [**Visual-UEFI-SHELL**](https://github.com/KilianKegel/Visual-UEFI-SHELL#edk2-uefi-shell--visual-uefi-shell)
* update `MdePkg` to version **edk2-stable202308**
* improve `CDEABI` (*C Development Environment Application Binary Interface*)<br>
    * force *all* `Core` and operating system interface `osif` modules to uses exclusively **ANSI-C-API** on `CDEABI` to avoid collision with EDK2 `StdLibC` and relatives<br>
      primarily remaining `errno()`, `setjmp()`, `longjmp()`, `strlen()`, `strcpy()`, `strcmp()`, `wcslen()`, `wcscpy()`, `wcscmp()`
* improve `freopen()` to set *redirection flag* `O_CDEREDIR` to speed up character transission to file
* fix `fscanf()`: assignment suppression indicated by a "*": https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf#page=295 
* fix `raise()`
* INTERN: 
	* add:  `size_t _cdeInt2EfiStatus(int intstatus)`
	* add:  `int _cdeEfiStatus2Int(size_t Status)`
	* move selected file functions to [`CdeAppIf`](https://github.com/KilianKegel/CdePkg/blob/master/Include/CdeServices.h#L421) driver side.<br>
	This allows future/upcoming code size reduced shell application type.
	* preliminary: LINUX-OSIF buildable, ALPHA
	* preliminary: `osifCdeUefiShellAppEntryPoint()`, `osifCdeUefiShellAppCRT0Service()` for future/upcoming shell application type
        that doesn't include entire `CdeAppIf` machine code to each .EFI application, but share `CdeAppIf` provided by a **CDE UEFI SHELL**.
        This allows code size reduced applications.

### 20230625
* add Standard C95 Library functions: 
	- [`wcstoul()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/Wcstoul.c)
	- [`wcstol()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/Wcstol.c)
	- [`mbrlen()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/mbrlen.c)
	- [`mbrtowc()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/mbrtowc.c)
	- [`mbsinit()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/mbsinit.c)
	- [`mbsrtowcs()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/mbsrtowcs.c)
	- [`wcrtomb()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/wcrtomb.c)
	- [`wcsftime()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/wcsftime.c)
	- [`wcsrtombs()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/wcsrtombs.c)
	- [`wscanf()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/wscanf.c)
* add Microsoft C Library functions from `mbctype.h`: 
    - [`_getmbcp()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/mbctype_h/_getmbcp.c)
    - [`_setmbcp()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/mbctype_h/_setmbcp.c)
* fixed: `printf()`/`wprintf()`-family handling of wide characters > value 255
* adjust internal `invalid_parameter_handler()` to suppress additional debug/file information
* fixed: `wcsxfrm()`/`strxfrm()` add `invalid_parameter_handler()` support as original Microsoft C Library

### 20230415
* fixed: in the pre-Memory-Discovered PEI (Pre Efi Initialization) POST x86-32
  Standard C function `localeconv()` crashed the platform.<br>
  NOTE: All x86-64 operation modes (UEFI Shell, UEFI SMM, UEFI DXE, Windows NT)
  and post-Memory-Discovered PEI (Pre Efi Initialization) x86-32 was not affected by that bug.
### 20230409
* add Memory Discovered (https://uefi.org/sites/default/files/resources/PI_Spec_1_6.pdf#page=111) handling for PEIM (Pre-EFI Initialization Module)
    * restart memory management when switching from CAR (Cash As RAM) to permanent memory
    * reassign `CDE_SERVICES` pointer when switching from CAR (Cash As RAM) to permanent memory
* support multi-invocation of `CdePkg`-based PEIM
    * NOTE: Each `CdePkg`-based PEIM needs a small, read-/writeable, dedicated static duration to provide
      Standard C Library compliance (e.g. to hold `atexit()`-registered function pointers, the internal `strtok()`pointer, the `rand()` next, the jump buffer,
      the I/O buffer for `stdin`, `stdout` and `stderr` ...).
      
      Internally this is provided in the HOB storage area and holds a `CDE_APP_IF` protocol registered with the driver specific `gEfiCallerIdGuid`.
      HOB storage is available early in POST in PEI (Pre-EFI Initialization).

      Once a PEIM is started multiple times, only the first instance static duration will be reinitialized with current `EFI_PEI_FILE_HANDLE`,
      `EFI_PEI_SERVICES` and the current `CDE_SERVICES` pointer.

      This proceeding prevents `LocatePpi()` to return an invalid pointer to the first instance, while a second instance is currently active.
      
* improve `CDEABI` (*C Development Environment Application Binary Interface*), used as collision avoidance with EDK2 `StdLibC` and relatives<br>
  NOTE: In real-world UEFI implementations various components provide [*reduced*](https://github.com/tianocore/edk2/blob/master/CryptoPkg/Library/BaseCryptLib/SysCall/CrtWrapper.c#L603)
  Standard C Interface just fitting the requirements of that particular package (`CryptoPkg`, `RedfishPkg`).<br>
  To avoid symbol double definitions at link time or link order failures , `CDEABI`:<br>
    * provides Standard C Functions in their `__declspec(dllimport)` incarnation only
    * except for Microsoft compiler intrinsics `__cdecl memset()` and `__cdecl memcmp()` that are paired with the its `__declsspec(dllimport)` counterpart in the same COMDAT (same .OBJ module)
### 20230304
* fixed `strtok()`, `wcstok()`
### 20230212
* introduce `CDEABI`(*C Development Environment Application Binary Interface*), an additional application binary interface ABI to ease coexistance of `CdePkg` based BIOS 
    drivers with incomplete [tianocore EDK2](https://github.com/tianocore/edk2) `C Library` 
    [fragments](https://github.com/tianocore/edk2/blob/master/CryptoPkg/Library/BaseCryptLib/SysCall/CrtWrapper.c#L603)

    NOTE: `CDEABI` uses the Microsoft DLL interface [`__declspec(dllimport)`](https://learn.microsoft.com/en-us/cpp/build/importing-into-an-application-using-declspec-dllimport?view=msvc-170) for EDK2-built drivers .
    Technically this uses *indirect function calls* on machine code level.
* promote `CDETRACE()`, remove former, alternate trace method (`CDEMOFINE()`) completely
* internally remove unused diagnostic interface
### 20230104
* fixed `strftime()` parameter: `%I`, `%m`, `%x`
### 20230103
* add Microsoft/POSIX C Library functions: 
    - `_isatty()`
* imitate funny/buggy Microsoft behaviour for `fopen()` with `fmode` parameter `w+` or `a`:<br>
  function terminates successfully with `errno` set to 22, `Invalid argument`
* imitate funny/buggy Microsoft behaviour for `_fileno()` with invalid filepointer:<br>
  MSFT: `_fileno(-1)` just crashes by an invalid memory access<br>
  This behaviour is imitated by an `exit(3)` invocation
* fixed application crash at termination when a redirected I/O stream `STDERR` is reopened with `reopen()`
* improve existing invalidate parameter handling; enable file name string, function name string, line number string and expression string
  at RELEASE runtime  
  NOTE: Microsoft enables this feature only when using DEBUG version of LIBCMT.LIB.
* internal: add **toro C Library** specific library extentions
    - `wchar_t* _wcsefierror(EFI_STATUS Status)`, according to Standard C `char* strerror(int errnum)`
    - `char* _strefierror(EFI_STATUS Status)`, according to Standard C `char* strerror(int errnum)`
### 20221022
* add O_TEMPORARY support to Microsoft/POSIX _open()
* fixed "fall time bug" (autumn). Broken time calculation on 
  two digit month number (Oct, Nov, Dec).
### 20221001
* simplify interaction of various **VISUAL-xyz** components, unitize directory structure 
  and import/export interface for those solutions
    * output folder for **Configuration Type: static Library** is commonly **$(SolutionPath)**\\***lib***
      to share one single *library folder* for all imported subprojects
    * adjust include path to be **$(ProjectDir)** relative
### 20220731
* add Standard C90 Library functions: 
    - `wcstoul()`
    - `wcstol()`
* add Standard C99 Library functions: 
    - `strtoull()`
    - `strtoll()`
    - `strtoimax()`
    - `strtoumax()`
    - `wcstoull()`
    - `wcstoll()`
    - `wcstoimax()`
    - `wcstoumax()`
* add Microsoft/POSIX C Library functions: 
    - `_ultow()`
    - `_ultoa()`
    - `_ui64tow()`
    - `_ui64toa()`
    - `_ltow()`
    - `_ltoa()`
    - `_itow()`
    - `_itoa()`
    - `_i64tow()`
    - `_i64toa()`
* implement full `__chkstk()` for Windows 32Bit: https://docs.microsoft.com/en-us/windows/win32/devnotes/-win32-chkstk
* fixed stdout/stderr redirection `>` and `>>` didn't work anymore (since `20220501`)
* fixed `"a"`/`O_APPEND` append file open attribute didn't work anymore (since `20220501`)
* fixed `ftell()`/`fgetpos()` reports wrong offset after writing to `"a"`/`O_APPEND` opened files

### 20220529
* fixed: `free()` and C++ operator `delete()`  crash to free `NULL` pointer

### 20220522
* add  C++ minimum support
    - `void* operator new(size_t size)`
    - `void* operator new[](size_t size)`
    - `void operator delete[](void* ptr)`
    - `void operator delete(void* ptr, unsigned __int64 size)`

### 20220501
* add Microsoft/POSIX C Library functions: 
    - `_open()`
    - `_close()`
    - `_read()`
    - `_write()`
    - `_fdopen()`
    - `_fileno()`
    - `_wfopen()`
    - `_fseeki64()`
    - `_ftelli64()`
* fixed `errno` values for file position functions with negative offsets (`fsetpos()`, `fseek()`)
* fixed UEFI BUG: gap of non-initialized disk space<br>
  File positioning bug, if data written behind EOF, data range between old EOF and new data contains medium data / garbage, instead of 0
* simplify `CDETRACE()` implemantation, improve portability of that `CdePkg` specific debug macro
* add `_strefierror()`: rename Toro-C-Library UEFI-specific function `strefierror()` to ANSI C naming convention compatible `_strefierror()`
* implement full `__chkstk()` for Windows 64Bit: https://docs.microsoft.com/en-us/windows/win32/devnotes/-win32-chkstk
* partially implementation of the %G `fprintf()`-- format specifier
* fixed `fread()` end-of-file indicator not set correctly when EOF is reached within a buffer instead of reading
  of 0 bytes from the mass storage device
* changed exit code of the `abort()` function from 3 to 0xC0000409
* fixed reassigned filepointer to `stdout`/`stderr` (by running `freopen()`) are not flushed at exit
* `CdePkg`--SMM Driver: Erroneous message shown at startup: `FATAL ERROR : CdeServices SMM not available`

### 20220115
* update header files with recently added functions
* change from solution relative to project relative build, adjust pathes
* update copyright

### 20220109
* add Standard C Library functions: 
    - `strcoll()`
    - `strxfrm()`
    - `fgetwc()`
    - `fputwc()`
    - `fputws()`
    - `fwide()`
    - `fwprintf()`
    - `fwscanf()`
    - `getwc()`
    - `getwchar()`
    - `putwc()`
    - `putwchar()`
    - `swscanf()`
    - `ungetwc()`
    - `vfwscanf()`
    - `vswscanf()`
    - `wcscoll()`
    - `wcsxfrm()`
    - `btowc()`
* fix "BINARY MODE" for wide printf()/scanf() family functions
* imitate funny Microsoft behaviour when replacing and pushing back  -`ungetc()`- a character by CTRL-Z to a stream (in that case the stream *is not terminated*)
* fix fscanf() family return value for "event of an early matching failure" http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=299
    
### 20211218
* introduce `CDETRACE()` debug/trace macro that is parameter checked at build time
* improve `wmain()` support; now selected at build time by choosing the CRT0 entry point name
    * `_cdeCRT0UefiShellW()`
    * `_cdeCRT0WinNTW()`
    NOTE: The `*env` pointer is not passed to `wmain()`
### 20211128
* fixed: UEFI Shell overwrites the last line of text of a previously terminated application with its prompt
* add: fgetws()

### 20211107
* add `wmain()` support
    ATTENTION: The presence of `main()` is not detected at build time anymore, but at runtime.

### 20211105
* initial release of the **toro C Library** source code
    * this is 4 years after providing **Torito C Library** in binary format: <br> https://github.com/KilianKegel/toro-C-Library#20171105r78

