# torosrc

[**toro C Library**](https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library)

```
    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0
```

## Revision history
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

