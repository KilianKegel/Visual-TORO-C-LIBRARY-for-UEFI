# torosrc

[**toro C Library**](https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library)

```
    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0
```

## Revision history
### 20220115
* update header files with recently added functions
* change from solution relative to project relative build, adjust pathes
* update copyright
### 20220109
* add Standard C Library functions: strcoll(), strxfrm(), fgetwc(), fputwc, fputws(), fwide(), fwprintf(),
  fwscanf(), getwc, getwchar(), putwc(), putwchar(), swscanf(), ungetwc(), vfwscanf(), vswscanf(), wcscoll(),
  wcsxfrm(), btowc()
* fix "BINARY MODE" for wide printf()/scanf() family functions
* imitate funny Microsoft behaviour when replacing and pushing back  -ungetc()- a character by CTRL-Z to a stream (in that case the stream *is not terminated*)
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

