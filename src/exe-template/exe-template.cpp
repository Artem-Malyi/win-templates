//
// exe-template.cpp
//

#include <Windows.h>

#ifdef _WIN64
    #define LOG_PREFIX "exe-template64"
#else
    #define LOG_PREFIX "exe-template32"
#endif
#define DEBUG_LOGGER_ENABLED
#define FILE_LOGGER_ENABLED
#include "logger.h"

void testDll();

BOOL WINAPI EntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    LOG("hInstance: 0x%0*x, dwReason: 0x%0*x, lpReserved: 0x%0*x", PTR_WIDTH, hinstDLL, PTR_WIDTH, fdwReason, PTR_WIDTH, lpReserved);

    __try {
        //
        // Place your code here.
        //
        testDll();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG("SEH exception occurred");
    }

    UINT uiStatus = ERROR_SUCCESS;
    LOG("Calling ExitProcess(%d)...", uiStatus);
    ExitProcess(uiStatus);

    return ERROR_SUCCESS;
}

void testDll() {
    #ifdef _WIN64
        #ifdef _DEBUG
            #define DLL_NAME L"dll-template64d.dll"
        #else
            #define DLL_NAME L"dll-template64.dll"
        #endif
    #else
        #ifdef _DEBUG
            #define DLL_NAME L"dll-template32d.dll"
        #else
            #define DLL_NAME L"dll-template32.dll"
        #endif
    #endif

    LoadLibrary(DLL_NAME);
    Sleep(2000);
}