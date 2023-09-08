//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxMemFree

\brief Implementation of the <em>osifUefiDxeEntryPoint</em> function

\file osifLinuxMemFree.c

*/
#include <CdeServices.h>

typedef unsigned long long EFI_PHYSICAL_ADDRESS;
//
// prototype
//
OSIFMEMFREE _osifLinuxMemFree;

void _osifLinuxMemFree(IN CDE_APP_IF* pCdeAppIf, IN EFI_PHYSICAL_ADDRESS Memory, IN size_t Pages) {

    return;//PEI can not release pages
}