//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxGetEnv

\brief Implementation of the <em>osifLinuxGetEnv</em> function

\file osifLinuxGetEnv.c

*/
#include <CdeServices.h>
//
// prototype
//
OSIFGETENV _osifLinuxGetEnv;

/*++

Routine Description:

    This function is the getenv()/Win32 interface

Arguments:


    @param[in] pstrEnvar - environment variable name

Return Value:

    @retval on SUCCESS: pointer to environment variable
    @retval on FAILURE: NULL

--*/
char* _osifLinuxGetEnv(IN CDE_APP_IF* pCdeAppIf, const char* pstrEnvar/*environment variable name*/)
{

    static char buffer[32769];
    char* pRet = buffer;

    //if (0 == GetEnvironmentVariable(pstrEnvar, buffer, sizeof(buffer)))
    //    pRet = NULL;


    return pRet;
}
