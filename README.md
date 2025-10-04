# *toro C Library* sourcecode

[**toro C Library**](https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library)

```
    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0
```
![](https://github.com/KilianKegel/pictures/blob/master/LLVMWyvernSmall2.png)


### Windows 11 SDK / Platform Toolset
 
||Windows SDK<BR>10.0.26100.0|Windows <BR>10.0.22621.0|Windows SDK<BR>10.0.19041.0|Windows SDK<BR>10.0.17763.0|Windows SDK<BR>10.0.14393.0|
|-|-|-|-|-|-|
|**Platform<br>toolset LLVM**|☑|☑|☑|☐|☐|
|**Platform<br>toolset v145<br>VS2026**|☑|☑|☑|☐|☐|
|**Platform<br>toolset v143<br>VS2022**|☑|☑|☑|☐|☐|
|**Platform<br>toolset v142<br>VS2019**|☑|☑|☑|☐|☐|
|**Platform<br>toolset v141<br>VS2017**|☑|☑|☑|☐|☐|
|**Platform<br>toolset v140<br>VS2010**|☐|☐|☐|☐|☐|

## Revision history
### 20251004, v0.9.8 Build 797
* fix **C++ `delete` operator** n/a in 32Bit library<br>
* default **toolset/SDK** configuration is now **VS2026 v145/10.0.26100.0** 
### 20250913, v0.9.7 Build 764
* fix [**errno.h**]((https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=198)) support for [**math.h** functions](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=224) in **UEFI POST drivers** (Power On Self Test)<br>
### 20250906, v0.9.6 Build 757
* fix buffer flush in `fprintf()`-family functions
### 20250830, v0.9.5 Build 755
* add [**errno.h**]((https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=198)) support for [**math.h** functions](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=224)
* publish **math.h** [**C-Validation Suite results**](https://github.com/KilianKegel/toroCVSreport?tab=readme-ov-file#mathh)

### 20250720, v0.9.4 Build 672
* **add LLVM/CLANG-cl-family support — the library source code itself is now buildable with LLVM/CLANG-cl:**
    * support VS2022 v17.14 with **LLVM (clang-cl)** tool chain
    * support **Intel C++ Compiler 2024** tool chain<br>
      **NOTE:** Only [**Intel oneAPI Base Toolkit ver 2024.2.1**](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html)
      provides the 32Bit compiler.<br>In versions **2025.x.y** the 32Bit compiler isn't shipped anymore.
* Windows SDK version **10.0.26100.0**
* support Microsoft tool chain:
    * **v143(VS2022)**
    * **v142(VS2019)** 
    * **v141(VS2017)**

### 20250712, v0.9.3 Build 298
* add C++ `virtual` function support by providing [Microsoft `type_info`](https://learn.microsoft.com/en-us/cpp/cpp/type-info-class?view=msvc-170) intrinsic

### 20250706, v0.9.2 Build 289
* fix fatal compiler optimization bug with  **Visual Studio 2022 v17.14**
    * [`memcpy()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/string_h/MemCpy.c) and [`wmemcpy()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/WMemCpy.c) recursively call `memcpy()`
### 20250517, v0.9.1 Build 269
* fix minor build error that appears with **Visual Studio 2022 v17.14**
* W/A for **cdeWelcomePei** sample driver when started with **gEfiPeiMemoryDiscoveredPpiGuid** depex hangs on some systems
 
### 20250322, v0.9.1 Build 267
* introduce **toro C Library** for the **Microsoft VS2022 LLVM tool chain** for **Windows NT** and **UEFI Shell** targets<br>
  **NOTE:** the Library binary is now useable with the **LLVM linker** **lld-link**.<br>
    - toroc64LLVMUefiShell.lib
    - toroc64LLVMWinNT.lib
    - toroc32LLVMUefiShell.lib
    - toroc32LLVMWinNT.lib

  **NOTE:** The LLVM linker **lld-link** can't deal with the multi-entrypoint library **toro C Library** directly.<br>
            To remedy this, the **toro C Library** is simply split into the above separate libraries.
* fix double float representation using printf()-**%G**-format specifier used in [**libxlsxwriter**](https://github.com/KilianKegel/Visual-LIBXLSXWRITER-for-UEFI-Shell#visual-libxlsxwriter-for-uefi-shell).
* fix [`pow()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/pow.c): **+/-infinity<sup>negative number</sup>**
* add missing import library version of [`wctob()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/wchar_h/wctobCDEABI.c)

 
    
### 20250309, v0.9.0 Build 243
#### finalize MATH.H implementation
<img src="https://upload.wikimedia.org/wikipedia/commons/0/06/KL_intel_i387DX.jpg" width="200"/><br>

The architecture of this math library introduces a space optimized implementation of C's MATH.H functions,
while keeping precision and correctness as good as already known in earlier x87-based math libraries.<BR>
Since the traditional math coprocessor 80387 is still present in current x86 processors and is also not deprecated
in the [X86S specification](https://www.intel.com/content/www/us/en/developer/articles/technical/envisioning-future-simplified-architecture.html) [.PDF](https://github.com/KilianKegel/4KPages-TechDocs/blob/main/x86s-eas-external-1.1.pdf),
it can be used here.<br><br>
The 80387 processor has various improvements over its 8087 predecessor, such as<br>
  * range extension for transcendental function:

    | Instruction  | function                      |
    |--------------|-------------------------------|
    |FPTAN         | Partial tangent               |
    |FPATAN        | Partial arctangent            |
    |F2XM1         | 2<sup>x</sup> - 1             |
    |FYL2X         | Y * log<sub>2</sub>X          |
    |FYL2XP1       |Y * log<sub>2</sub>(X + 1)     |
    
* new instructions, e.g.

    | Instruction  | function                 |
    |--------------|--------------------------|
    |FXAM          |  Examine Floating-Point  |
    |FSIN          | sine                     |
    |FCOS          | cosine                   |

That has reduced the programming effort dramatically and made algorithms very simple and easy to implement.<br>

* [`acos()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/acos.c)
* [`asin()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/asin.c)
* [`atan()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/atan.c)
* [`atan2()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/atan2.c)
* [`ceil()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/ceil.c)
* [`cos()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/cos.c)
* [`cosh()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/cosh.c)
* [`exp()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/exp.c)
* [`fabs()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/fabs.c)
* [`floor()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/floor.c)
* [`fmod()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/fmod.c)
* [`frexp()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/frexp.c)
* [`ldexp()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/ldexp.c)
* [`log()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/log.c)
* [`log10()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/log10.c)
* [`modf()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/modf.c)
* [`pow()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/pow.c)
* [`sin()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/sin.c)
* [`sinh()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/sinh.c)
* [`sqrt()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/sqrt.c)
* [`tan()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/tan.c)    
* [`tanh()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/math_h/tanh.c)

**All these functions run also in UEFI POST stages PEI, DXE, SMM.**

### 20250222, v0.8.9 Build 231
* fix build error with **Windows SDK 10.0.26100.0**
### 20241222, v0.8.9 Build 227
* fix [`abort()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/stdlib_h/abort.c)
  processes [`atexit()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/stdlib_h/atexit.c)
  registered functions and closes files before exit.
* fix [`memset()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/string_h/MemSetCDEINTRINABI.c)
  to do solely 8Bit memory machine operations.
* add Microsoft C Library functions: 
    - [`_exit()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/stdlib_h/_exit.c)
* fix minor symbol naming error
### 20241109
* INTERN: minor correction
### 20241103, v0.8.8 Build 222
* add dedicated DEBUG/RELEASE support for **CdePkg**.<br>
  In preparation for the upcoming [**EDK2-EMU**](https://github.com/KilianKegel/EDK2-EMU) project
  the entire **CdePkg** and **CdePkg**-based components will be debugable on source code level in the **EDK2-Emulator**.
### 20241013, v0.8.7 Build 200
* fixed: removed  disassembled math function wasn't replaced by extracted intrinsic 
math function from Microsoft **LIBCMT.LIB**:
    * `ftol3.obj`
    * `ullshr.obj`
    * `ullrem.obj`
    * `ulldvrm.obj`
    * `ulldiv.obj`
    * `llshr.obj`
    * `llshl.obj`
    * `llrem.obj`
    * `llmul.obj`
    * `lldvrm.obj`
    * `lldiv.obj`
    original Microsoft functions are now available in the **toro C Library** for 32Bit.
* **NEW**: Introduce **preliminary alpha** version of `MATH.H` functions<br>
### 20240908, v0.8.6 Build 187
* introduce intrinsic math function (`_allXYZ()`, `_aullXYZ()` and `__ltod3()`) extraction from Microsoft **LIBCMT.LIB<br>
    * remove disassembled `__allXYZ()` and `__aullXYZ()` from `toroCLibrary`
* fixed `stat()` running on WINDOWS reports wrong time stamp
* fixed `stat()` running on UEFI reports from erronous time stamp by exacly one month
* fixed `stat()` running on UEFI to support MSDOS drive name (**A:**, **B:**, **C:** ...)
* fixed `system()` running on UEFI with output redirection emits additional garbage characters
### 20240804, v0.8.5 Build 122
* improve **C++** support for `CDE.H`
    * enable mixed *C/C++* applications based **toro C Library** using `CDE.H`
* fixe SMM issues:
    * fix `CdePkg`-based SMM drivers hangs on startup.
    * fix `CdeServicesSmm.c` failed to build<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**

### 20240505, v0.8.4 Build 91
* fixed EDK2 `DEBUG` trace macro won't crash anymore with
  UEFI specific (non-ANSI-C) format specifiers: **%g**, **%t** and **%r**<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**
### 20240428, v0.8.3 Build 84
* include static code analysis configuration `toroCLibrary.ruleset`
* fix minor static code analysis warnings<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**
### 20240414, v0.8.2 Build 73
* add timeout detection for disabled **COM1**/**UART at I/O 3F8h** used for debug traces.<br>
  NOTE: Disabled I/O devices usually do not respond to I/O cycles.  
  Internally this is done by ignoring the chipselect for that particular I/O address range, e.g. 3F8h .. 3FFh for **COM1**.  
  FFh is driven to the data bus when reading registers of those disabled devices.<br>
  On special implementations hardware designers chose a different approach to disabled devices:<br>
    1. address decoding is kept enabled
    2. internal clock line is stopped or decoupled from internal circuitry<br>
       The disadvantage of this aproach is, that status registers are still visible
       but not updated anymore.<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**
### 20240309, v0.8.1 Build 54
* add Microsoft/POSIX C Library functions: 
    - [`_lseek()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/io_h/_Lseek.c)
    - [`_eof()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/io_h/_Eof.c)
### 20240212, v0.8.0 Build 1
* add semantic versioning + build enumeration
* optimize source code
    * reduce number of suppressed warnings (4200;4324;4100 only)
    * enable static code analysis (`toroCLibrary.ruleset`, suppress warning 28251 only )<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**
### 20231228
* update copyright date<br>
**NOTE: The improvement above doesn't change ANSI-C related behaviour of previous library versions**
### 20231118
* add ACPI timer based synchronization for **toro C Library** Shell programs.<br>
  NOTE: On recent Intel platforms the previously used legacy timer's (i8254) input clock frequency is
  clocked down to an unspecified frequency with setup default `Enable 8254 Clock Gate`.
  Additionally the I/O latency to access i8254 ports is increased with setup default `Legacy IO Low Latency`
  that lowers i8254 based timing precision.<br>
  So i8254 gets unusable for UEFI Shell programs on new platforms.
* improve synchronization error correction for i8254 based POST drivers (with `Enable 8254 Clock Gate := disable`, `Legacy IO Low Latency := enable`)
### 20231014
* fixed: [**libxlsxwriter**](https://github.com/KilianKegel/Visual-LIBXLSXWRITER-for-UEFI-Shell#visual-libxlsxwriter-for-uefi-shell) based
    UEFI and Windows applications create .XLSX that can't be opened with **Microsoft Excel 2016**. 
    Office 365 online (https://www.microsoft365.com) and offline didn't fail.
### 20230926
* fixed: [`_strefieerror()`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/toroCLibrary/Library/string_h/_StrEfiError.c) to return error correct string when running in pre-memory PEI
### 20230917
* fix `CdePkg\Include\CDE.h` for debug trace purpose in conjunction with 
    original Microsoft header files 
### 20230916
* improve debug trace CDETRACE() configuration switches
    * `#define CDEDBG STDOUT` – traces directed to stdout
    * `#define CDEDBG STDERR` – traces directed to stderr
    * `#define CDEDBG STDDBG` – traces directed to CDE debug channel, normally COM1, I/O 0x3F8 115200,8,n,1
    * `CDEDBG` undefined – UEFI Shell/POST DRIVERS: STDDBG, Windows NT: STDOUT

### 20230909
* add `CDE`(*C Development Environment*)-interface for native **Tianocore UEFI SHELL** [`UEFISHELLDRV`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/tree/main/toroCLibrary/OSInterface/UEFISHELLDRV).<br>
  Enable [**Toro-C-Library**–`DXE`](https://github.com/KilianKegel/toro-C-Library#implementation-status) function set at `CRT0()`
  and full [**Toro-C-Library**–`SHELL`](https://github.com/KilianKegel/toro-C-Library#implementation-status) function set with availability
  of the `EfiShellProtocol`/`EFI_SHELL_PROTOCOL_GUID`:<br>
  https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/tree/main/toroCLibrary/OSInterface/UEFISHELLDRV<br>

  This is the foundation of an **ANSI-C-API**-extended "**CDE UEFI SHELL**"

* prepare `system()` **ANSI-C-API** call for MSDOS drive name support (**A:**, **B:**, **C:** ...) coming soon for [**Visual-UEFI-SHELL**](https://github.com/KilianKegel/Visual-UEFI-SHELL#edk2-uefi-shell--visual-uefi-shell)
* update [`MdePkg`](https://github.com/KilianKegel/MdePkgInc) to version **edk2-stable202308**
* improve [`CDEABI`](https://github.com/KilianKegel/Visual-TORO-C-LIBRARY-for-UEFI/blob/main/README.md#20230212) (*C Development Environment Application Binary Interface*)<br>
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
  and POST-Memory-Discovered PEI (Pre Efi Initialization) x86-32 was not affected by that bug.
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

