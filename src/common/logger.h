#pragma once

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


// Below are the definitions that turn on/off the logging. Define them prior to #include "logger.h" line.
//#define LOGGER_ENABLED
//#define FILE_LOGGER_ENABLED

// Default settings. May be overridden with definitions prior to #include "logger.h" line.
#ifndef LOG_MUTEX_NAME
    #define LOG_MUTEX_NAME    L"Global\\058070A4-C80F-4098-A359-7B22B9D53CAF"
#endif

#ifndef LOGFILE_NAME
    // will be found on:
    //      WinXP: in %tmp%
    //      WinXP SYSTEM account: in %windir%\TEMP
    //      Win7:  in %appdata%\LocalLow
    //      Win7 SYSTEM account: in %windir%\System32\config\systemprofile\AppData\LocalLow
    #define LOGFILE_NAME       L"log.txt" 
#endif

#ifndef LOG_PREFIX
    #define LOG_PREFIX "DEFINE_LOG_PREFIX!"
#endif

// BOOL to STRING macro
#define B2S(bFlag) (bFlag ? "TRUE" : "FALSE")

// Actual macros
#ifdef FILE_LOGGER_ENABLED
    extern "C" void __cdecl __Log2File(LPSTR string2Log);
    #define LOG_TO_FILE(sText) __Log2File(sText); 
#else
    #define LOG_TO_FILE(sText)
#endif

#ifdef _WIN64
    #define PTR_WIDTH 16
#else
    #define PTR_WIDTH 8
#endif

#define OUTPUT_BUFFER_SIZE 1024    

#ifdef LOGGER_ENABLED
    #define LOG(fmt, ...)                                                                                                   \
    {                                                                                                                       \
        static SYSTEMTIME __st = {0};                                                                                       \
        GetSystemTime(&__st);                                                                                               \
        static CHAR __sImageName[MAX_PATH] = {0};                                                                           \
        GetModuleFileNameA(GetModuleHandleA(NULL), __sImageName, MAX_PATH);                                                 \
        PathStripPathA(__sImageName);                                                                                       \
        static CHAR __sOutput[OUTPUT_BUFFER_SIZE] = {0};                                                                    \
        typedef int (__stdcall *_snprintf_t)(char* buffer, size_t count, const char* format, ...);                          \
        _snprintf_t pSnprintf = (_snprintf_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "_snprintf");                    \
        if (pSnprintf) {                                                                                                    \
            pSnprintf(__sOutput, OUTPUT_BUFFER_SIZE, "%02d/%02d/%d %02d:%02d:%02d.%03d %s [%d:%d] %s: %s - "##fmt##"\r\n",  \
                __st.wMonth, __st.wDay, __st.wYear, __st.wHour, __st.wMinute, __st.wSecond, __st.wMilliseconds,             \
                __sImageName, GetCurrentProcessId(), GetCurrentThreadId(), LOG_PREFIX, __FUNCTION__, __VA_ARGS__);          \
            if (!StrStrIA(__sImageName, "dbgview.exe"))                                                                     \
                OutputDebugStringA(__sOutput);                                                                                  \
            LOG_TO_FILE(__sOutput);                                                                                         \
        }                                                                                                                   \
    }
#else 
    #define LOG(...)
#endif

// UNCONDITIONAL_LOG
#define UNC_LOG(fmt, ...)                                                                                               \
{                                                                                                                       \
    static SYSTEMTIME __st = {0};                                                                                       \
    GetSystemTime(&__st);                                                                                               \
    static CHAR __sImageName[MAX_PATH] = {0};                                                                           \
    GetModuleFileNameA(GetModuleHandleA(NULL), __sImageName, MAX_PATH);                                                 \
    PathStripPathA(__sImageName);                                                                                       \
    static CHAR __sOutput[OUTPUT_BUFFER_SIZE] = {0};                                                                    \
    typedef int (__stdcall *_snprintf_t)(char* buffer, size_t count, const char* format, ...);                          \
    _snprintf_t pSnprintf = (_snprintf_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "_snprintf");                    \
    if (pSnprintf) {                                                                                                    \
        pSnprintf(__sOutput, OUTPUT_BUFFER_SIZE, "%02d/%02d/%d %02d:%02d:%02d.%03d %s [%d:%d]: %s - "##fmt##"\r\n",     \
            __st.wMonth, __st.wDay, __st.wYear, __st.wHour, __st.wMinute, __st.wSecond, __st.wMilliseconds,             \
            __sImageName, GetCurrentProcessId(), GetCurrentThreadId(), __FUNCTION__, __VA_ARGS__);                      \
        if (!StrStrIA(__sImageName, "dbgview.exe"))                                                                     \
            OutputDebugStringA(__sOutput);                                                                                  \
        LOG_TO_FILE(__sOutput);                                                                                         \
    }                                                                                                                   \
}