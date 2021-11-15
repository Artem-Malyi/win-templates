// 
// source.cpp
//

#include <Windows.h>

#ifdef _WIN64
    #define LOG_PREFIX "exe-template64.exe"
#else
    #define LOG_PREFIX "exe-template32.exe"
#endif
#define DEBUG_LOGGER_ENABLED
//#define FILE_LOGGER_ENABLED
#include "logger.h"

BOOL WINAPI EntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    LOG("hInstance: 0x%0*x, dwReason: 0x%0*x, lpReserved: 0x%0*x", PTR_WIDTH, hinstDLL, PTR_WIDTH, fdwReason, PTR_WIDTH, lpReserved);

    //
    // Place your code here.
    //

    UINT uiStatus = ERROR_SUCCESS;
    LOG("Calling ExitProcess(%d)...", uiStatus);
    ExitProcess(uiStatus);

    return ERROR_SUCCESS;
}