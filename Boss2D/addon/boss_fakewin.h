#pragma once
#include <boss.h>

#pragma warning(disable:4005) // macro redefinition

#if !defined(_MSC_VER) || (_MSC_VER < 1900)
    #define snprintf(...) boss_snprintf(__VA_ARGS__)
#endif
#define WIN32
#define _WIN32
#define __WIN32__
#define _WIN32_WINNT 0x0501
#define WINDOWS
#define _WINDOWS
#define NDEBUG
#if !BOSS_WINDOWS
    #define _MSC_VER 1900
    #undef __linux__
    #undef __APPLE__
    #undef ANDROID
    #undef __ANDROID__
#endif

#define BOSS_FAKEWIN_DECLSPEC_DLLEXPORT //__declspec(dllexport)
#define BOSS_FAKEWIN_DECLSPEC_DLLIMPORT //__declspec(dllimport)
#define BOSS_FAKEWIN_STDCALL            //__stdcall

////////////////////////////////////////////////////////////////////////////////////////////////////
// _BOSS_BATCH_COMMAND_ {"type" : "include-alias", "prm" : "BOSS_FAKEWIN_", "restore-comment" : " //original-code:"}
#if BOSS_WINDOWS & !BOSS_NEED_FORCED_FAKEWIN
    #define BOSS_FAKEWIN_V_windows_h                       <windows.h>
    #define BOSS_FAKEWIN_V_io_h                            <io.h>
    #define BOSS_FAKEWIN_V_basetsd_h                       <basetsd.h>
    #define BOSS_FAKEWIN_V_winsock_h                       <winsock.h>
    #define BOSS_FAKEWIN_V_winsock2_h                      <winsock2.h>
    #define BOSS_FAKEWIN_V_direct_h                        <direct.h>
    #define BOSS_FAKEWIN_V_ws2tcpip_h                      <ws2tcpip.h>
    #define BOSS_FAKEWIN_V_malloc_h                        <malloc.h>
    #define BOSS_FAKEWIN_V_tchar_h                         <tchar.h>
    #define BOSS_FAKEWIN_V_shlwapi_h                       <shlwapi.h>
    #define BOSS_FAKEWIN_V_wincon_h                        <wincon.h>
    #define BOSS_FAKEWIN_V_conio_h                         <conio.h>
    #define BOSS_FAKEWIN_V_wincrypt_h                      <wincrypt.h>
    #define BOSS_FAKEWIN_V_tlhelp32_h                      <tlhelp32.h>
    #define BOSS_FAKEWIN_V_lmcons_h                        <lmcons.h>
    #define BOSS_FAKEWIN_V_lmstats_h                       <lmstats.h>
    #define BOSS_FAKEWIN_V_stdint_h                        <stdint.h>
    #define BOSS_FAKEWIN_U_stdint_h                        "stdint.h"
    #define BOSS_FAKEWIN_V_winbase_h                       <winbase.h>
    #define BOSS_FAKEWIN_V_windef_h                        <windef.h>
    #define BOSS_FAKEWIN_V_process_h                       <process.h>
    #define BOSS_FAKEWIN_V_mbstring_h                      <mbstring.h>
    #define BOSS_FAKEWIN_V_search_h                        <search.h>
    #define BOSS_FAKEWIN_V_intrin_h                        <intrin.h>

    // 선행참조효과
    #undef UNICODE
    #include <winsock2.h>
    #include <windows.h>

    typedef struct fd_set boss_fakewin_fd_set;
    #define GetCurrentDirectoryW boss_fakewin_GetCurrentDirectoryW
    typedef unsigned short mode_t;
#else
    #define BOSS_COMMAND_OBJECTS_IS_ENABLED
    #define BOSS_FAKEWIN_V_windows_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_io_h                            <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_basetsd_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_winsock_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_winsock2_h                      <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_direct_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_ws2tcpip_h                      <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_malloc_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_tchar_h                         <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_shlwapi_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_wincon_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_conio_h                         <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_wincrypt_h                      <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_tlhelp32_h                      <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_lmcons_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_lmstats_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_stdint_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_U_stdint_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_winbase_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_windef_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_process_h                       <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_mbstring_h                      <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_search_h                        <addon/boss_fakewin.h>
    #define BOSS_FAKEWIN_V_intrin_h                        <addon/boss_fakewin.h>

    #define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
    DECLARE_HANDLE(HWINSTA);
    DECLARE_HANDLE(HINSTANCE);
    DECLARE_HANDLE(HKEY);
    DECLARE_HANDLE(HDC);
    DECLARE_HANDLE(HGDIOBJ);
    #define HBITMAP HGDIOBJ

    typedef char CHAR;
    typedef char TCHAR, *PTCHAR;
    typedef char* LPSTR;
    typedef const char *LPCSTR, *PCSTR;
    typedef unsigned char UCHAR;
    typedef unsigned short USHORT;
    typedef wchar_t* LPWSTR;
    typedef const wchar_t* LPCWSTR;
    typedef unsigned char BYTE, *LPBYTE;
    typedef wchar_t WCHAR;
    typedef LPCSTR LPCTSTR;
    typedef LPSTR LPTSTR;
    typedef unsigned short WORD;
    typedef long LONG, *PLONG, *LPLONG;
    typedef long long LONGLONG;
    typedef unsigned long long ULONGLONG;
    typedef unsigned long ULONG;
    typedef int BOOL, *LPBOOL;
    typedef int INT, *LPINT;
    typedef unsigned int UINT;
    typedef long long INT64;
    typedef unsigned long long UINT64;
    #if BOSS_X64
        typedef long long INT_PTR;
        typedef unsigned long long UINT_PTR;
        typedef long long LONG_PTR;
        typedef unsigned long long ULONG_PTR;
    #else
        typedef int INT_PTR;
        typedef unsigned int UINT_PTR;
        typedef long LONG_PTR;
        typedef unsigned long ULONG_PTR;
    #endif
    typedef unsigned long DWORD, *LPDWORD;
    typedef unsigned long DWORD_PTR, *PDWORD_PTR;
    typedef unsigned long long DWORDLONG;
    typedef void VOID;
    typedef void* PVOID;
    typedef void* LPVOID;
    typedef const void* LPCVOID;
    typedef void* HANDLE;
    typedef unsigned int SOCKET;
    typedef USHORT ADDRESS_FAMILY;
    typedef int socklen_t;
    typedef HANDLE HWND;
    typedef HANDLE HLOCAL;
    typedef HINSTANCE HMODULE;
    typedef HKEY* PHKEY;
    typedef DWORD ACCESS_MASK;
    typedef ACCESS_MASK REGSAM;
    typedef UINT_PTR WPARAM;
    typedef LONG_PTR LPARAM;
    typedef LONG_PTR LRESULT;
    typedef int UNKNOWN;
    typedef int (*FARPROC)();

    #define LPOVERLAPPED void*
    #define LPSECURITY_ATTRIBUTES void*
    #define LPTHREAD_START_ROUTINE void*

    #define __int8 char
    #define __int16 short
    #define __int32 int
    #define __int64 long long
    #define u_short USHORT
    typedef unsigned int u_int;
    typedef int errno_t;
    typedef unsigned short mode_t;
    typedef unsigned int _dev_t;
    typedef unsigned short _ino_t;
    typedef long _off_t;
    typedef __int64 __time64_t;

    // 선행참조효과(공통)
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>
    #include <ctype.h>
    #include <errno.h>
    #include <sys/stat.h>
    #include <wchar.h>
    #if !BOSS_MAC_OSX && !BOSS_IPHONE
        #include <malloc.h>
    #endif
    #if BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        #include <sys/select.h>
    #endif
    #if BOSS_WINDOWS
        #include <direct.h>
        #include <io.h>
        #include <conio.h>
    #endif
    #include <stdint.h>
    #include <fcntl.h>

    #ifdef __cplusplus
        #if !BOSS_MAC_OSX && !BOSS_IPHONE
            #include <iostream>
            #include <fstream>
            #include <sstream>
        #endif
    #endif

    #define socket boss_fakewin_socket
    #define connect boss_fakewin_connect
    #define getsockopt boss_fakewin_getsockopt
    #define select boss_fakewin_select
    #define recv boss_fakewin_recv
    #define send boss_fakewin_send
    #define closesocket boss_fakewin_closesocket
    #define ioctlsocket boss_fakewin_ioctlsocket
    #define gethostbyname boss_fakewin_gethostbyname
    #define htons boss_fakewin_htons
    #define getaddrinfo boss_fakewin_getaddrinfo
    #define freeaddrinfo boss_fakewin_freeaddrinfo
    #define gai_strerror boss_fakewin_gai_strerror
    #define getpid boss_fakewin_getpid
    #define mbstowcs_s boss_fakewin_mbstowcs_s
    #define wcstombs_s boss_fakewin_wcstombs_s

    #define GetCurrentDirectoryW boss_fakewin_GetCurrentDirectoryW
    #define CreateFileW boss_fakewin_CreateFileW
    #define CreateFileMapping boss_fakewin_CreateFileMapping
    #define CreateDirectory boss_fakewin_CreateDirectory
    #define RemoveDirectory boss_fakewin_RemoveDirectory
    #define CloseHandle boss_fakewin_CloseHandle
    #define MapViewOfFile boss_fakewin_MapViewOfFile
    #define UnmapViewOfFile boss_fakewin_UnmapViewOfFile
    #define CreateThread boss_fakewin_CreateThread
    #define DeregisterEventSource boss_fakewin_DeregisterEventSource
    #define DeviceIoControl boss_fakewin_DeviceIoControl
    #define ExpandEnvironmentStringsW boss_fakewin_ExpandEnvironmentStringsW
    #define FindClose boss_fakewin_FindClose
    #define FindFirstFileA boss_fakewin_FindFirstFileA
    #define FindFirstFileW boss_fakewin_FindFirstFileW
    #define FindFirstFileExA boss_fakewin_FindFirstFileExA
    #define FindNextFileA boss_fakewin_FindNextFileA
    #define FindNextFileW boss_fakewin_FindNextFileW
    #define FindWindow boss_fakewin_FindWindow
    #define FlushConsoleInputBuffer boss_fakewin_FlushConsoleInputBuffer
    #define FlushFileBuffers boss_fakewin_FlushFileBuffers
    #define FormatMessageW boss_fakewin_FormatMessageW
    #define GetCurrentProcess boss_fakewin_GetCurrentProcess
    #define GetCurrentThreadId boss_fakewin_GetCurrentThreadId
    #define GetExitCodeThread boss_fakewin_GetExitCodeThread
    #define GetFileAttributes boss_fakewin_GetFileAttributes
    #define GetFileAttributesW boss_fakewin_GetFileAttributesW
    #define GetFileAttributesExA boss_fakewin_GetFileAttributesExA
    #define GetFileAttributesExW boss_fakewin_GetFileAttributesExW
    #define GetFullPathNameA boss_fakewin_GetFullPathNameA
    #define GetFullPathNameW boss_fakewin_GetFullPathNameW
    #define GetLastError boss_fakewin_GetLastError
    #define SetLastError boss_fakewin_SetLastError
    #define LoadLibraryA boss_fakewin_LoadLibraryA
    #define GetModuleHandleA boss_fakewin_GetModuleHandleA
    #define GetModuleHandleW boss_fakewin_GetModuleHandleW
    #define GetModuleFileName boss_fakewin_GetModuleFileName
    #define GetProcAddress boss_fakewin_GetProcAddress
    #define GetProcessAffinityMask boss_fakewin_GetProcessAffinityMask
    #define GetProcessWindowStation boss_fakewin_GetProcessWindowStation
    #define GetProcessTimes boss_fakewin_GetProcessTimes
    #define GetShortPathNameW boss_fakewin_GetShortPathNameW
    #define GetStdHandle boss_fakewin_GetStdHandle
    #define GetSystemInfo boss_fakewin_GetSystemInfo
    #define GetNativeSystemInfo boss_fakewin_GetNativeSystemInfo
    #define GetSystemTimeAsFileTime boss_fakewin_GetSystemTimeAsFileTime
    #define SystemTimeToFileTime boss_fakewin_SystemTimeToFileTime
    #define GetTickCount boss_fakewin_GetTickCount
    #define GetUserObjectInformationW boss_fakewin_GetUserObjectInformationW
    #define LocalFree boss_fakewin_LocalFree
    #define MessageBox boss_fakewin_MessageBox
    #define CopyFile boss_fakewin_CopyFile
    #define DeleteFileA boss_fakewin_DeleteFileA
    #define MoveFileEx boss_fakewin_MoveFileEx
    #define MoveFileExW boss_fakewin_MoveFileExW
    #define GetTempPathA boss_fakewin_GetTempPathA
    #define GetTempFileNameA boss_fakewin_GetTempFileNameA
    #define MultiByteToWideChar boss_fakewin_MultiByteToWideChar
    #define WideCharToMultiByte boss_fakewin_WideCharToMultiByte
    #define RegCloseKey boss_fakewin_RegCloseKey
    #define RegisterEventSource boss_fakewin_RegisterEventSource
    #define RegOpenKeyExW boss_fakewin_RegOpenKeyExW
    #define RegQueryValueExW boss_fakewin_RegQueryValueExW
    #define ReportEvent boss_fakewin_ReportEvent
    #define SecureZeroMemory boss_fakewin_SecureZeroMemory
    #define SendMessage boss_fakewin_SendMessage
    #define SetFileAttributesW boss_fakewin_SetFileAttributesW
    #define Sleep boss_fakewin_Sleep
    #define VerifyVersionInfo boss_fakewin_VerifyVersionInfo
    #define SetEvent boss_fakewin_SetEvent
    #define ResetEvent boss_fakewin_ResetEvent
    #define CreateEvent boss_fakewin_CreateEvent
    #define CreateEventW boss_fakewin_CreateEventW
    #define WaitForSingleObject boss_fakewin_WaitForSingleObject
    #define WaitForMultipleObjects boss_fakewin_WaitForMultipleObjects
    #define DeleteCriticalSection boss_fakewin_DeleteCriticalSection
    #define TryEnterCriticalSection boss_fakewin_TryEnterCriticalSection
    #define EnterCriticalSection boss_fakewin_EnterCriticalSection
    #define InitializeCriticalSection boss_fakewin_InitializeCriticalSection
    #define LeaveCriticalSection boss_fakewin_LeaveCriticalSection
    #define WSAGetLastError boss_fakewin_WSAGetLastError
    #define WSASetLastError boss_fakewin_WSASetLastError
    #define WSACleanup boss_fakewin_WSACleanup
    #define WSAStartup boss_fakewin_WSAStartup
    #define WSAStringToAddressA boss_fakewin_WSAStringToAddressA
    #define QueryPerformanceFrequency boss_fakewin_QueryPerformanceFrequency
    #define QueryPerformanceCounter boss_fakewin_QueryPerformanceCounter
    #define ZeroMemory boss_fakewin_ZeroMemory
    #define GetObject boss_fakewin_GetObject
    #define DeleteObject boss_fakewin_DeleteObject
    #define GetDC boss_fakewin_GetDC
    #define ReleaseDC boss_fakewin_ReleaseDC
    #define CreateDIBSection boss_fakewin_CreateDIBSection
    #define TlsAlloc boss_fakewin_TlsAlloc
    #define TlsFree boss_fakewin_TlsFree
    #define TlsGetValue boss_fakewin_TlsGetValue
    #define TlsSetValue boss_fakewin_TlsSetValue
    #define CreateMutexA boss_fakewin_CreateMutexA
    #define ReleaseMutex boss_fakewin_ReleaseMutex
    #define CreateSemaphoreA boss_fakewin_CreateSemaphoreA
    #define ReleaseSemaphore boss_fakewin_ReleaseSemaphore
    #define GetStartupInfo boss_fakewin_GetStartupInfo
    #define CreateProcessA boss_fakewin_CreateProcessA
    // 유니코드여부관련
    #define DeleteFile DeleteFileA
    #define GetTempPath GetTempPathA
    #define FindFirstFile FindFirstFileA
    #define FindNextFile FindNextFileA
    #define CreateMutex CreateMutexA
    #define CreateSemaphore CreateSemaphoreA
    #define CreateProcess CreateProcessA
    #define GetModuleHandle GetModuleHandleA

    #define _waccess boss_fakewin_waccess
    #define _wchmod boss_fakewin_wchmod
    #define _wfopen boss_fakewin_wfopen
    #define _wfopen_s boss_fakewin_wfopen_s
    #define _fopen boss_fakewin_fopen
    #define _fopen_s boss_fakewin_fopen_s
    #define _fseek boss_fakewin_fseek
    #define _ftell boss_fakewin_ftell
    #define _rewind boss_fakewin_rewind
    #define _fread boss_fakewin_fread
    #define _fwrite boss_fakewin_fwrite
    #define _fgetc boss_fakewin_fgetc
    #define _fgets boss_fakewin_fgets
    #define _ungetc boss_fakewin_ungetc
    #define _fclose boss_fakewin_fclose
    #define _wopen boss_fakewin_wopen
    #define _close boss_fakewin_close
    #define _read boss_fakewin_read
    #define _write boss_fakewin_write
    #define _get_osfhandle boss_fakewin_get_osfhandle
    #define _lseek boss_fakewin_lseek
    #define _lseeki64 boss_fakewin_lseeki64
    #define _lfind boss_fakewin_lfind
    #define _chsize_s boss_fakewin_chsize_s
    #define _fstat64 boss_fakewin_fstat64
    #define _wchdir boss_fakewin_wchdir
    #define _wmkdir boss_fakewin_wmkdir
    #define _wrmdir boss_fakewin_wrmdir
    #define _wunlink boss_fakewin_wunlink
    #define _mktemp_s boss_fakewin_mktemp_s
    #define _wgetenv boss_fakewin_wgetenv
    #define _wgetcwd boss_fakewin_wgetcwd
    #define FD_SET boss_fakewin_FD_SET
    #define FD_ZERO boss_fakewin_FD_ZERO
    #define _alloca boss_fakewin_alloca
    #define _fileno boss_fakewin_fileno
    #define _getch boss_fakewin_getch
    #define _getdrive boss_fakewin_getdrive
    #define _lrotl boss_fakewin_lrotl
    #define _rotl boss_fakewin_rotl
    #define _setmode boss_fakewin_setmode
    #define _stricmp boss_fakewin_stricmp
    #define _strnicmp boss_fakewin_strnicmp
    #define _mbsicmp boss_fakewin_mbsicmp
    #define _wcsicmp boss_fakewin_wcsicmp
    #define _vscprintf boss_fakewin_vscprintf
    #define _vsnprintf_s boss_fakewin_vsnprintf_s
    #define _vsnprintf boss_fakewin_vsnprintf
    #define _vsnwprintf boss_fakewin_vsnwprintf
    #define _beginthreadex boss_fakewin_beginthreadex
    #define _fullpath boss_fakewin_fullpath
    #define _ltoa boss_fakewin_ltoa
    #define _ultoa boss_fakewin_ultoa
    #define _isnan boss_fakewin_isnan
    #define _finite boss_fakewin_finite
    #define _splitpath boss_fakewin_splitpath
    #define _strtoi64 boss_fakewin_strtoi64
    #define _strtoui64 boss_fakewin_strtoui64
    #define _set_errno boss_fakewin_set_errno
    // 명칭재정의관련
    #define wfopen _wfopen
    #define wfopen_s _wfopen_s
    #define fopen _fopen
    #define fopen_s _fopen_s
    #define fseek _fseek
    #define ftell _ftell
    #define rewind _rewind
    #define fread _fread
    #define fwrite _fwrite
    #define fgetc _fgetc
    #define fgets _fgets
    #define ungetc _ungetc
    #define fclose _fclose
    #define ltoa _ltoa
    #define _snprintf boss_snprintf
    #ifdef UNICODE
        #define _vsntprintf _vsnwprintf
    #else
        #define _vsntprintf _vsnprintf
    #endif

    typedef struct {
        #define FD_SETSIZE 64
        u_int fd_count;               /* how many are SET? */
        SOCKET fd_array[FD_SETSIZE];   /* an array of SOCKETs */
    } boss_fakewin_fd_set;
    #define fd_set boss_fakewin_fd_set

    #undef st_atime
    #undef st_mtime
    #undef st_ctime
    #if BOSS_WINDOWS
        struct boss_fakewin_stat64 {
            _dev_t     st_dev;
            _ino_t     st_ino;
            unsigned short st_mode;
            short      st_nlink;
            short      st_uid;
            short      st_gid;
            _dev_t     st_rdev;
            __int64    st_size;
            __time64_t st_atime;
            __time64_t st_mtime;
            __time64_t st_ctime;
        };
    #else
        #define boss_fakewin_stat64 stat
    #endif

    #ifdef __cplusplus
        extern "C" {
    #endif
        int boss_fakewin_waccess(const wchar_t*,int);
        int boss_fakewin_wchmod(const wchar_t*,int);
        FILE* boss_fakewin_wfopen(const wchar_t*,const wchar_t*);
        errno_t boss_fakewin_wfopen_s(FILE**, const wchar_t*, const wchar_t*);
        FILE* boss_fakewin_fopen(char const*, char const*);
        errno_t boss_fakewin_fopen_s(FILE**, char const*, char const*);
        int boss_fakewin_fseek(FILE*,long int,int);
        long int boss_fakewin_ftell(FILE*);
        void boss_fakewin_rewind(FILE*);
        size_t boss_fakewin_fread(void*,size_t,size_t,FILE*);
        size_t boss_fakewin_fwrite(const void*,size_t,size_t,FILE*);
        int boss_fakewin_fgetc(FILE* stream);
        char* boss_fakewin_fgets(char*,int,FILE*);
        int boss_fakewin_ungetc(int,FILE*);
        int boss_fakewin_fclose(FILE*);
        int boss_fakewin_wopen(const wchar_t*,int,int);
        int boss_fakewin_close(int);
        long boss_fakewin_read(int, void*, unsigned int);
        long boss_fakewin_write(int, const void*, unsigned int);
        intptr_t boss_fakewin_get_osfhandle(int);
        long boss_fakewin_lseek(int, long, int);
        __int64 boss_fakewin_lseeki64(int,__int64,int);
        void* boss_fakewin_lfind(const void*,const void*,unsigned int*,unsigned int,int (*)(const void*, const void*));
        errno_t boss_fakewin_chsize_s(int, __int64);
        int boss_fakewin_fstat64(int,struct boss_fakewin_stat64*);
        int boss_fakewin_wchdir(const wchar_t*);
        int boss_fakewin_wmkdir(const wchar_t*);
        int boss_fakewin_wrmdir(const wchar_t*);
        int boss_fakewin_wunlink(const wchar_t*);
        errno_t boss_fakewin_mktemp_s(char*,size_t);
        wchar_t* boss_fakewin_wgetenv(const wchar_t*);
        wchar_t* boss_fakewin_wgetcwd(wchar_t*,int);
        void boss_fakewin_FD_SET(int fd, boss_fakewin_fd_set* fdset);
        void boss_fakewin_FD_ZERO(boss_fakewin_fd_set* fdset);
        void* boss_fakewin_alloca(size_t);
        int boss_fakewin_fileno(FILE*);
        int boss_fakewin_getch();
        int boss_fakewin_getdrive();
        unsigned long boss_fakewin_lrotl(unsigned long,int);
        unsigned int boss_fakewin_rotl(unsigned int,int);
        int boss_fakewin_setmode(int,int);
        int boss_fakewin_stricmp(const char*,const char*);
        int boss_fakewin_strnicmp(const char*,const char*,size_t);
        int boss_fakewin_mbsicmp(const unsigned char*, const unsigned char*);
        int boss_fakewin_wcsicmp(wchar_t const*,wchar_t const*);
        int boss_fakewin_vscprintf(const char*,va_list);
        int boss_fakewin_vsnprintf_s(char*,size_t,size_t,const char*,va_list);
        int boss_fakewin_vsnprintf(char*,size_t,const char*,va_list);
        int boss_fakewin_vsnwprintf(wchar_t*,size_t,const wchar_t*,va_list);
        uintptr_t boss_fakewin_beginthreadex(void*, unsigned, unsigned (*)(void*), void*, unsigned, unsigned*);
        char* boss_fakewin_fullpath(char*,const char*,size_t);
        char* boss_fakewin_ltoa(long,char*,int);
        char* boss_fakewin_ultoa(unsigned long,char*,int);
        int boss_fakewin_isnan(double);
        int boss_fakewin_finite(double);
        void boss_fakewin_splitpath(const char*,char*,char*,char*,char*);
        __int64 boss_fakewin_strtoi64(const char*,char**,int);
        unsigned __int64 boss_fakewin_strtoui64(const char*,char**,int);
        errno_t boss_fakewin_set_errno(int);
    #ifdef __cplusplus
        }
    #endif

    #if BOSS_WINDOWS
        typedef unsigned long u_long;
        typedef long long longlong_t;
        typedef unsigned long long u_longlong_t;
        //typedef __int8 int8_t;
        //typedef __int16 int16_t;
        //typedef __int32 int32_t;
        //typedef __int64 int64_t;
        //typedef unsigned __int8 uint8_t;
        //typedef unsigned __int16 uint16_t;
        //typedef unsigned __int32 uint32_t;
        //typedef unsigned __int64 uint64_t;
        typedef long long intmax_t;
        typedef unsigned long long uintmax_t;
        #define INT8_MIN         (-127i8 - 1)
        #define INT16_MIN        (-32767i16 - 1)
        #define INT32_MIN        (-2147483647i32 - 1)
        #define INT64_MIN        (-9223372036854775807i64 - 1)
        #define INT8_MAX         127i8
        #define INT16_MAX        32767i16
        #define INT32_MAX        2147483647i32
        #define INT64_MAX        9223372036854775807i64
        #define UINT8_MAX        0xffui8
        #define UINT16_MAX       0xffffui16
        #define UINT32_MAX       0xffffffffui32
        #define UINT64_MAX       0xffffffffffffffffui64

        struct timeval {
            long    tv_sec;         /* seconds */
            long    tv_usec;        /* and microseconds */
        };
        #  if defined(_UNICODE) || defined(__UNICODE__)
        #   define _vsntprintf _vsnwprintf
        #  else
        #   define _vsntprintf _vsnprintf
        #  endif
    #elif BOSS_MAC_OSX || BOSS_IPHONE
        typedef unsigned long u_long;
        #ifdef __cplusplus
            namespace std
            {
                #define stringstream boss_fakewin_stringstream
                class boss_fakewin_stringstream
                {
                public:
                    stringstream();
                    ~stringstream();
                    void str(const char* s);
                private:
                    void* mStr;
                };
                #define ios_base boss_fakewin_ios_base
                class boss_fakewin_ios_base
                {
                public:
                    enum openmode {in, binary};
                };
            }
        #endif
        #define __POCC__ 0
        #define __POCC__OLDNAMES
    #endif

    #ifdef __cplusplus
        namespace std
        {
            #define ifstream boss_fakewin_ifstream
            class boss_fakewin_ifstream : public stringstream
            {
                FILE* f;
            public:
                boss_fakewin_ifstream(const char* filename, ios_base::openmode mode = ios_base::in);
                ~boss_fakewin_ifstream();
                bool is_open() const;
                void close();
            };
        }
    #endif

    #define WINAPI
    #define _inline
    #define __cdecl
    #define _MAX_PATH                       260
    #define _MAX_FNAME                      256
    #define VER_MINORVERSION                0x0000001
    #define VER_MAJORVERSION                0x0000002
    #define VER_BUILDNUMBER                 0x0000004
    #define VER_PLATFORMID                  0x0000008
    #define VER_SERVICEPACKMINOR            0x0000010
    #define VER_SERVICEPACKMAJOR            0x0000020
    #define VER_SUITENAME                   0x0000040
    #define VER_PRODUCT_TYPE                0x0000080
    #define VER_EQUAL                       1
    #define VER_GREATER                     2
    #define VER_GREATER_EQUAL               3
    #define VER_LESS                        4
    #define VER_LESS_EQUAL                  5
    #define VER_AND                         6
    #define VER_OR                          7
    #define VER_CONDITION_MASK              7
    #define VER_NUM_BITS_PER_CONDITION_MASK 3
    #define VER_SET_CONDITION(_m_, _t_, _c_)
    #define MAX_PATH                        260
    #define _TRUNCATE                       ((size_t) -1)
    #define INVALID_HANDLE_VALUE            ((HANDLE) -1)
    #define INVALID_SOCKET                  ((SOCKET) ~0)
    #define STD_INPUT_HANDLE                ((DWORD) -10)
    #define SOCKET_ERROR                    (-1)
    #define AF_UNSPEC                       0
    #define AF_INET                         2
    #define AF_INET6                        23
    #define SOCK_STREAM                     1
    #define IPPROTO_IP                      0
    #define IPPROTO_ICMP                    1
    #define IPPROTO_TCP                     6
    #define IPPROTO_UDP                     17
    #define WSADESCRIPTION_LEN              256
    #define WSASYS_STATUS_LEN               128
    #define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
    #define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
    #define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
    #define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
    #define ERROR_FILE_NOT_FOUND             2L
    #define ERROR_ACCESS_DENIED              5L
    #define ERROR_INVALID_HANDLE             6L
    #define ERROR_NOT_ENOUGH_MEMORY          8L
    #define ERROR_NO_MORE_FILES              18L
    #define ERROR_SHARING_VIOLATION          32L
    #define ERROR_INSUFFICIENT_BUFFER        122L
    #define ERROR_DIRECTORY                  267L
    #define ERROR_INVALID_FLAGS              1004L
    #define ERROR_NO_UNICODE_TRANSLATION     1113L
    #define EVENTLOG_ERROR_TYPE              0x0001
    #define WM_COPYDATA                      0x004A
    #define SOL_SOCKET                       0xffff
    #define SO_ERROR                         0x1007
    #define WSAEINTR                         10004L
    #define WSAEINVAL                        10022L
    #define WSAEFAULT                        10014L
    #define WSAEWOULDBLOCK                   10035L
    #define WSAENOTSOCK                      10038L
    #define WSA_NOT_ENOUGH_MEMORY   (ERROR_NOT_ENOUGH_MEMORY)
    #define MB_OK                       0x00000000L
    #define MB_ICONHAND                 0x00000010L
    #define MB_ICONERROR                MB_ICONHAND
    #define MB_PRECOMPOSED            0x00000001  // use precomposed chars
    #define MB_COMPOSITE              0x00000002  // use composite chars
    #define MB_USEGLYPHCHARS          0x00000004  // use glyph chars, not ctrl chars
    #define MB_ERR_INVALID_CHARS      0x00000008  // error for invalid chars
    #define WC_ERR_INVALID_CHARS      0x00000080
    #define IOCPARM_MASK    0x7f            /* parameters must be < 128 bytes */
    #define IOC_IN          0x80000000      /* copy in parameters */
    #define _IOW(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
    #define FIONBIO         _IOW('f', 126, u_long) /* set/clear non-blocking i/o */
    #define LANG_NEUTRAL                     0x00
    #define SUBLANG_DEFAULT                             0x01    // user default
    #define READ_CONTROL                     (0x00020000L)
    #define STANDARD_RIGHTS_READ             (READ_CONTROL)
    #define SYNCHRONIZE                      (0x00100000L)
    #define KEY_QUERY_VALUE         (0x0001)
    #define KEY_ENUMERATE_SUB_KEYS  (0x0008)
    #define KEY_NOTIFY              (0x0010)
    #define KEY_READ                ((STANDARD_RIGHTS_READ|KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_NOTIFY) & (~SYNCHRONIZE))
    #define REG_SZ                      ( 1 )
    #define HKEY_CURRENT_USER                   (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
    #define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
    #define PAGE_READWRITE         0x04
    #define PAGE_READONLY          0x02
    #define SECTION_MAP_WRITE            0x0002
    #define SECTION_MAP_READ             0x0004
    #define FILE_MAP_WRITE      SECTION_MAP_WRITE
    #define FILE_MAP_READ       SECTION_MAP_READ
    #define MAXIMUM_REPARSE_DATA_BUFFER_SIZE      ( 16 * 1024 )
    #define GENERIC_READ                     (0x80000000L)
    #define OPEN_EXISTING       3
    #define IO_REPARSE_TAG_MOUNT_POINT              (0xA0000003L)
    #define FILE_SHARE_READ                 0x00000001
    #define FILE_SHARE_WRITE                0x00000002
    #define FILE_SHARE_DELETE               0x00000004
    #define FILE_ATTRIBUTE_READONLY             0x00000001
    #define FILE_ATTRIBUTE_HIDDEN               0x00000002
    #define FILE_ATTRIBUTE_DIRECTORY            0x00000010
    #define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
    #define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
    #define FILE_FLAG_BACKUP_SEMANTICS      0x02000000
    #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
    #define S_IFDIR            0x4000          /* directory */
    #define S_IFREG            0x8000          /* regular */
    #define S_IREAD            0x0100          /* read permission, owner */
    #define S_IWRITE           0x0080          /* write permission, owner */
    #define MOVEFILE_REPLACE_EXISTING       0x00000001
    #define MOVEFILE_COPY_ALLOWED           0x00000002
    #define INFINITE            0xFFFFFFFF  // Infinite timeout
    #define STATUS_WAIT_0       ((DWORD) 0x00000000L)
    #define WAIT_OBJECT_0       ((STATUS_WAIT_0 ) + 0)
    #define FALSE               0
    #define TRUE                1
    #define CP_ACP                    0
    #define CP_UTF8             65001
    #define UOI_NAME        2
    #define O_BINARY       0x8000
    #define O_NOINHERIT    0x0080
    #define O_TEMPORARY    0x0040  /* temporary file bit */
    #define O_RDONLY       0x0000
    #define O_WRONLY       0x0001  /* open for writing only */
    #define O_RDWR         0x0002
    #define O_APPEND       0x0008  /* writes done at eof */
    #define O_CREAT        0x0100  /* create and open file */
    #define O_TRUNC        0x0200  /* open and truncate */
    #define    O_EXCL         0x0400    /* Open only if the file does not exist. */
    #define    O_UNKNOWN      0x0800
    #define O_TEXT         0x4000
    #define O_CLOEXEC      0x80000
    #define _O_BINARY      O_BINARY
    #define _O_NOINHERIT   O_NOINHERIT
    #define _O_TEMPORARY   O_TEMPORARY
    #define _O_RDONLY      O_RDONLY
    #define _O_WRONLY      O_WRONLY
    #define _O_RDWR        O_RDWR
    #define _O_APPEND      O_APPEND
    #define _O_CREAT       O_CREAT
    #define _O_TRUNC       O_TRUNC
    #define    _O_EXCL        O_EXCL
    #define    _O_UNKNOWN     O_UNKNOWN
    #define _O_TEXT        O_TEXT
    #define _O_CLOEXEC     O_CLOEXEC
    #define _SS_MAXSIZE 128                 // Maximum size
    #define _SS_ALIGNSIZE (sizeof(__int64)) // Desired alignment
    #define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof (short))
    #define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof (short) + _SS_PAD1SIZE + _SS_ALIGNSIZE))
    #define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
    #define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
    #define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
    #define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
    #define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
    #define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
    #define MAKELANGID(p, s)    ((((WORD)(s)) << 10) | (WORD)(p))
    #define PRIMARYLANGID(lgid) ((WORD)(lgid) & 0x3ff)
    #define SUBLANGID(lgid)     ((WORD)(lgid) >> 10)
    #define _T(STR) STR
    #define ANYSIZE_ARRAY 1
    #define MAX_PROTOCOL_CHAIN 7
    #define WSAPROTOCOL_LEN  255
    #define BI_RGB        0L
    #define BI_RLE8       1L
    #define BI_RLE4       2L
    #define BI_BITFIELDS  3L
    #define BI_JPEG       4L
    #define BI_PNG        5L
    #define DIB_RGB_COLORS      0 /* color table in RGBs */
    #define DIB_PAL_COLORS      1 /* color table in palette indices */
    #define CREATE_BREAKAWAY_FROM_JOB         0x01000000
    #define CREATE_PRESERVE_CODE_AUTHZ_LEVEL  0x02000000
    #define CREATE_DEFAULT_ERROR_MODE         0x04000000
    #define CREATE_NO_WINDOW                  0x08000000
    #define DEBUG_PROCESS                     0x00000001
    #define DEBUG_ONLY_THIS_PROCESS           0x00000002
    #define CREATE_SUSPENDED                  0x00000004
    #define DETACHED_PROCESS                  0x00000008

    typedef struct tagCOPYDATASTRUCT {
        ULONG_PTR dwData;
        DWORD cbData;
        PVOID lpData;
    } COPYDATASTRUCT, *PCOPYDATASTRUCT;

    typedef enum _GET_FILEEX_INFO_LEVELS {
        GetFileExInfoStandard,
        GetFileExMaxInfoLevel
    } GET_FILEEX_INFO_LEVELS;

    typedef enum _FINDEX_INFO_LEVELS { 
      FindExInfoStandard,
      FindExInfoBasic,
      FindExInfoMaxInfoLevel
    } FINDEX_INFO_LEVELS;

    typedef enum _FINDEX_SEARCH_OPS { 
      FindExSearchNameMatch,
      FindExSearchLimitToDirectories,
      FindExSearchLimitToDevices
    } FINDEX_SEARCH_OPS;

    typedef struct _OSVERSIONINFOEX {
        DWORD dwOSVersionInfoSize;
        DWORD dwMajorVersion;
        DWORD dwMinorVersion;
        DWORD dwBuildNumber;
        DWORD dwPlatformId;
        CHAR  szCSDVersion[128];
        WORD  wServicePackMajor;
        WORD  wServicePackMinor;
        WORD  wSuiteMask;
        BYTE  wProductType;
        BYTE  wReserved;
    } OSVERSIONINFOEX, *LPOSVERSIONINFOEX;

    typedef struct _WSAData {
            WORD                    wVersion;
            WORD                    wHighVersion;
            #if BOSS_X64
                unsigned short          iMaxSockets;
                unsigned short          iMaxUdpDg;
                char *                  lpVendorInfo;
                char                    szDescription[WSADESCRIPTION_LEN+1];
                char                    szSystemStatus[WSASYS_STATUS_LEN+1];
            #else
                char                    szDescription[WSADESCRIPTION_LEN+1];
                char                    szSystemStatus[WSASYS_STATUS_LEN+1];
                unsigned short          iMaxSockets;
                unsigned short          iMaxUdpDg;
                char *                  lpVendorInfo;
            #endif
    } WSADATA, *LPWSADATA;

    typedef struct in_addr {
        union {
            struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
            struct { USHORT s_w1,s_w2; } S_un_w;
            ULONG S_addr;
        } S_un;
        #define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
        #define s_host  S_un.S_un_b.s_b2    // host on imp
        #define s_net   S_un.S_un_b.s_b1    // network
        #define s_imp   S_un.S_un_w.s_w2    // imp
        #define s_impno S_un.S_un_b.s_b4    // imp #
        #define s_lh    S_un.S_un_b.s_b3    // logical host
    } IN_ADDR, *PIN_ADDR, *LPIN_ADDR;

    typedef struct in6_addr {
        union {
            UCHAR       Byte[16];
            USHORT      Word[8];
        } u;
    } IN6_ADDR, *PIN6_ADDR, *LPIN6_ADDR;

    typedef struct {
        union {
            struct {
                ULONG Zone : 28;
                ULONG Level : 4;
            };
            ULONG Value;
        };
    } SCOPE_ID, *PSCOPE_ID;

    typedef struct sockaddr {
        #if (_WIN32_WINNT < 0x0600)
            u_short sa_family;
        #else
            ADDRESS_FAMILY sa_family;           // Address family.
        #endif //(_WIN32_WINNT < 0x0600)
        CHAR sa_data[14];                   // Up to 14 bytes of direct address.
    } SOCKADDR, *PSOCKADDR, *LPSOCKADDR;

    struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
    };

    typedef struct sockaddr_storage {
        ADDRESS_FAMILY ss_family;      // address family
        CHAR __ss_pad1[_SS_PAD1SIZE];  // 6 byte pad, this is to make
                                        //   implementation specific pad up to
                                        //   alignment field that follows explicit
                                        //   in the data structure
        __int64 __ss_align;            // Field to force desired structure
        CHAR __ss_pad2[_SS_PAD2SIZE];  // 112 byte pad to achieve desired size;
                                        //   _SS_MAXSIZE value minus size of
                                        //   ss_family, __ss_pad1, and
                                        //   __ss_align fields is 112
    } SOCKADDR_STORAGE_LH, *PSOCKADDR_STORAGE_LH, *LPSOCKADDR_STORAGE_LH;

    typedef struct _FILETIME {
        DWORD dwLowDateTime;
        DWORD dwHighDateTime;
    } FILETIME, *LPFILETIME;

    typedef struct _SYSTEMTIME {
      WORD wYear;
      WORD wMonth;
      WORD wDayOfWeek;
      WORD wDay;
      WORD wHour;
      WORD wMinute;
      WORD wSecond;
      WORD wMilliseconds;
    } SYSTEMTIME, *PSYSTEMTIME;

    typedef union _LARGE_INTEGER {
      struct {
        DWORD LowPart;
        LONG  HighPart;
      };
      struct {
        DWORD LowPart;
        LONG  HighPart;
      } u;
      LONGLONG QuadPart;
    } LARGE_INTEGER, *PLARGE_INTEGER;

    typedef union _ULARGE_INTEGER {
      struct {
        DWORD LowPart;
        DWORD HighPart;
      };
      struct {
        DWORD LowPart;
        DWORD HighPart;
      } u;
      ULONGLONG QuadPart;
    } ULARGE_INTEGER, *PULARGE_INTEGER;

    typedef struct tagBITMAP {
      LONG   bmType;
      LONG   bmWidth;
      LONG   bmHeight;
      LONG   bmWidthBytes;
      WORD   bmPlanes;
      WORD   bmBitsPixel;
      LPVOID bmBits;
    } BITMAP, *PBITMAP;

    typedef struct tagBITMAPINFOHEADER {
      DWORD biSize;
      LONG  biWidth;
      LONG  biHeight;
      WORD  biPlanes;
      WORD  biBitCount;
      DWORD biCompression;
      DWORD biSizeImage;
      LONG  biXPelsPerMeter;
      LONG  biYPelsPerMeter;
      DWORD biClrUsed;
      DWORD biClrImportant;
    } BITMAPINFOHEADER, *PBITMAPINFOHEADER;

    typedef struct tagRGBQUAD {
      BYTE rgbBlue;
      BYTE rgbGreen;
      BYTE rgbRed;
      BYTE rgbReserved;
    } RGBQUAD;

    typedef struct tagBITMAPINFO {
      BITMAPINFOHEADER bmiHeader;
      RGBQUAD          bmiColors[1];
    } BITMAPINFO, *PBITMAPINFO, *LPBITMAPINFO;

    typedef struct tagDIBSECTION {
      BITMAP           dsBm;
      BITMAPINFOHEADER dsBmih;
      DWORD            dsBitfields[3];
      HANDLE           dshSection;
      DWORD            dsOffset;
    } DIBSECTION, *PDIBSECTION;

    typedef struct _GUID {
        DWORD Data1;
        WORD  Data2;
        WORD  Data3;
        BYTE  Data4[8];
    } GUID;

    typedef struct _STARTUPINFO {
      DWORD  cb;
      LPTSTR lpReserved;
      LPTSTR lpDesktop;
      LPTSTR lpTitle;
      DWORD  dwX;
      DWORD  dwY;
      DWORD  dwXSize;
      DWORD  dwYSize;
      DWORD  dwXCountChars;
      DWORD  dwYCountChars;
      DWORD  dwFillAttribute;
      DWORD  dwFlags;
      WORD   wShowWindow;
      WORD   cbReserved2;
      LPBYTE lpReserved2;
      HANDLE hStdInput;
      HANDLE hStdOutput;
      HANDLE hStdError;
    } STARTUPINFO, *LPSTARTUPINFO;

    typedef struct _PROCESS_INFORMATION {
      HANDLE hProcess;
      HANDLE hThread;
      DWORD  dwProcessId;
      DWORD  dwThreadId;
    } PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

    typedef struct _RTL_CRITICAL_SECTION {
        UNKNOWN DebugInfo;
        LONG LockCount;
        LONG RecursionCount;
        HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
        HANDLE LockSemaphore;
        ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
    } RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
    typedef RTL_CRITICAL_SECTION CRITICAL_SECTION, *LPCRITICAL_SECTION;

    typedef struct _SID_IDENTIFIER_AUTHORITY {
        BYTE Value[6];
    } SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;

    typedef struct _SID {
        BYTE  Revision;
        BYTE  SubAuthorityCount;
        SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
        DWORD SubAuthority[ANYSIZE_ARRAY];
    } SID, *PSID;

    typedef struct _SYSTEM_INFO {
        union {
            DWORD dwOemId;          // Obsolete field...do not use
            struct {
                WORD wProcessorArchitecture;
                WORD wReserved;
            } DUMMYSTRUCTNAME;
        } DUMMYUNIONNAME;
        DWORD dwPageSize;
        LPVOID lpMinimumApplicationAddress;
        LPVOID lpMaximumApplicationAddress;
        DWORD_PTR dwActiveProcessorMask;
        DWORD dwNumberOfProcessors;
        DWORD dwProcessorType;
        DWORD dwAllocationGranularity;
        WORD wProcessorLevel;
        WORD wProcessorRevision;
    } SYSTEM_INFO, *LPSYSTEM_INFO;

    typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
    } WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

    typedef struct _WIN32_FIND_DATAA {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
        DWORD dwReserved0;
        DWORD dwReserved1;
        CHAR  cFileName[MAX_PATH];
        CHAR  cAlternateFileName[14];
        #if BOSS_MAC_OSX
            DWORD dwFileType;
            DWORD dwCreatorType;
            WORD  wFinderFlags;
        #endif
    } WIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
    #define WIN32_FIND_DATA WIN32_FIND_DATAA

    typedef struct _WIN32_FIND_DATAW {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
        DWORD dwReserved0;
        DWORD dwReserved1;
        WCHAR cFileName[MAX_PATH];
        WCHAR cAlternateFileName[14];
        #if BOSS_MAC_OSX
            DWORD dwFileType;
            DWORD dwCreatorType;
            WORD  wFinderFlags;
        #endif
    } WIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;

    typedef struct _WSABUF {
        ULONG len;     /* the length of the buffer */
        CHAR* buf;     /* the pointer to the buffer */
    } WSABUF, *LPWSABUF;

    typedef struct sockaddr_in6 {
        ADDRESS_FAMILY sin6_family; // AF_INET6.
        USHORT sin6_port;           // Transport level port number.
        ULONG  sin6_flowinfo;       // IPv6 flow information.
        IN6_ADDR sin6_addr;         // IPv6 address.
        union {
            ULONG sin6_scope_id;     // Set of interfaces for a scope.
            SCOPE_ID sin6_scope_struct;
        };
    } SOCKADDR_IN6_LH, *PSOCKADDR_IN6_LH, *LPSOCKADDR_IN6_LH;

    typedef struct hostent {
        char*         h_name;
        char**        h_aliases;
        short         h_addrtype;
        short         h_length;
        char**        h_addr_list;
    } HOSTENT, *PHOSTENT, *LPHOSTENT;

    typedef struct servent {
        char*         s_name;
        char**        s_aliases;
        short         s_port;
        char*         s_proto;
    } SERVENT, *PSERVENT, *LPSERVENT;

    typedef struct addrinfo {
        struct hostent *ai_hostent;
        struct servent *ai_servent;
        struct sockaddr_in ai_addr_in;
        struct sockaddr *ai_addr;
        size_t ai_addrlen;
        int ai_family;
        int ai_socktype;
        int ai_protocol;
        long ai_port;
        struct addrinfo *ai_next;
    } ADDRINFOA, *PADDRINFOA;

    typedef struct _WSAPROTOCOLCHAIN {
        int   ChainLen;
        DWORD ChainEntries[MAX_PROTOCOL_CHAIN];
    } WSAPROTOCOLCHAIN, *LPWSAPROTOCOLCHAIN;

    typedef struct _WSAPROTOCOL_INFO {
        DWORD            dwServiceFlags1;
        DWORD            dwServiceFlags2;
        DWORD            dwServiceFlags3;
        DWORD            dwServiceFlags4;
        DWORD            dwProviderFlags;
        GUID             ProviderId;
        DWORD            dwCatalogEntryId;
        WSAPROTOCOLCHAIN ProtocolChain;
        int              iVersion;
        int              iAddressFamily;
        int              iMaxSockAddr;
        int              iMinSockAddr;
        int              iSocketType;
        int              iProtocol;
        int              iProtocolMaxOffset;
        int              iNetworkByteOrder;
        int              iSecurityScheme;
        DWORD            dwMessageSize;
        DWORD            dwProviderReserved;
        TCHAR            szProtocol[WSAPROTOCOL_LEN+1];
    } WSAPROTOCOL_INFO, *LPWSAPROTOCOL_INFO;

    #ifdef __cplusplus
        extern "C" {
    #endif
        SOCKET boss_fakewin_socket(int, int, int);
        int boss_fakewin_connect(SOCKET, const struct sockaddr*, int);
        int boss_fakewin_getsockopt(SOCKET, int, int, char*, int*);
        int boss_fakewin_select(int, boss_fakewin_fd_set*, boss_fakewin_fd_set*, boss_fakewin_fd_set*, const struct timeval*);
        int boss_fakewin_recv(SOCKET, char*, int, int);
        int boss_fakewin_send(SOCKET, const char*, int, int);
        int boss_fakewin_closesocket(SOCKET);
        int boss_fakewin_ioctlsocket(SOCKET,long,u_long*);
        struct hostent* boss_fakewin_gethostbyname(const char*);
        unsigned short boss_fakewin_htons(unsigned short);
        int boss_fakewin_getaddrinfo(PCSTR,PCSTR,const ADDRINFOA*,PADDRINFOA*);
        void boss_fakewin_freeaddrinfo(struct addrinfo*);
        const char* boss_fakewin_gai_strerror(int);
        int boss_fakewin_getpid();
        errno_t boss_fakewin_mbstowcs_s(size_t*,wchar_t*,size_t,const char*,size_t);
        errno_t boss_fakewin_wcstombs_s(size_t*,char*,size_t,const wchar_t*,size_t);

        HANDLE boss_fakewin_CreateFileW(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
        HANDLE boss_fakewin_CreateFileMapping(HANDLE,LPSECURITY_ATTRIBUTES,DWORD,DWORD,DWORD,LPCTSTR);
        BOOL boss_fakewin_CreateDirectory(LPCSTR,LPSECURITY_ATTRIBUTES);
        BOOL boss_fakewin_RemoveDirectory(LPCSTR);
        BOOL boss_fakewin_CloseHandle(HANDLE);
        LPVOID boss_fakewin_MapViewOfFile(HANDLE,DWORD,DWORD,DWORD,size_t);
        BOOL boss_fakewin_UnmapViewOfFile(LPCVOID);
        HANDLE boss_fakewin_CreateThread(LPSECURITY_ATTRIBUTES,size_t,LPTHREAD_START_ROUTINE,LPVOID,DWORD,LPDWORD);
        BOOL boss_fakewin_DeregisterEventSource(HANDLE);
        BOOL boss_fakewin_DeviceIoControl(HANDLE,DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD,LPOVERLAPPED);
        DWORD boss_fakewin_ExpandEnvironmentStringsW(LPCWSTR,LPWSTR,DWORD);
        BOOL boss_fakewin_FindClose(HANDLE);
        HANDLE boss_fakewin_FindFirstFileA(LPCSTR,LPWIN32_FIND_DATAA);
        HANDLE boss_fakewin_FindFirstFileW(LPCWSTR,LPWIN32_FIND_DATAW);
        HANDLE boss_fakewin_FindFirstFileExA(LPCSTR,FINDEX_INFO_LEVELS,LPVOID,FINDEX_SEARCH_OPS,LPVOID,DWORD);
        BOOL boss_fakewin_FindNextFileA(HANDLE,LPWIN32_FIND_DATAA);
        BOOL boss_fakewin_FindNextFileW(HANDLE,LPWIN32_FIND_DATAW);
        HWND boss_fakewin_FindWindow(LPCTSTR,LPCTSTR);
        BOOL boss_fakewin_FlushConsoleInputBuffer(HANDLE);
        BOOL boss_fakewin_FlushFileBuffers(HANDLE);
        DWORD boss_fakewin_FormatMessageW(DWORD,LPCVOID,DWORD,DWORD,LPWSTR,DWORD,va_list*);
        HANDLE boss_fakewin_GetCurrentProcess();
        DWORD boss_fakewin_GetCurrentThreadId();
        BOOL boss_fakewin_GetExitCodeThread(HANDLE,LPDWORD);
        DWORD boss_fakewin_GetFileAttributes(LPCSTR);
        DWORD boss_fakewin_GetFileAttributesW(LPCWSTR);
        BOOL boss_fakewin_GetFileAttributesExA(LPCSTR,GET_FILEEX_INFO_LEVELS,LPVOID);
        BOOL boss_fakewin_GetFileAttributesExW(LPCWSTR,GET_FILEEX_INFO_LEVELS,LPVOID);
        DWORD boss_fakewin_GetFullPathNameA(LPCSTR,DWORD,LPSTR,LPSTR*);
        DWORD boss_fakewin_GetFullPathNameW(LPCWSTR,DWORD,LPWSTR,LPWSTR*);
        DWORD boss_fakewin_GetLastError();
        void boss_fakewin_SetLastError(DWORD);
        HMODULE boss_fakewin_LoadLibraryA(LPCSTR);
        HMODULE boss_fakewin_GetModuleHandleA(LPCSTR);
        HMODULE boss_fakewin_GetModuleHandleW(LPCWSTR);
        DWORD boss_fakewin_GetModuleFileName(HMODULE,LPSTR,DWORD);
        FARPROC boss_fakewin_GetProcAddress(HMODULE,LPCSTR);
        BOOL boss_fakewin_GetProcessAffinityMask(HANDLE,PDWORD_PTR,PDWORD_PTR);
        HWINSTA boss_fakewin_GetProcessWindowStation();
        BOOL boss_fakewin_GetProcessTimes(HANDLE,LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);
        DWORD boss_fakewin_GetShortPathNameW(LPCWSTR,LPWSTR,DWORD);
        HANDLE boss_fakewin_GetStdHandle(DWORD);
        void boss_fakewin_GetSystemInfo(LPSYSTEM_INFO);
        void boss_fakewin_GetNativeSystemInfo(LPSYSTEM_INFO);
        void boss_fakewin_GetSystemTimeAsFileTime(LPFILETIME);
        BOOL boss_fakewin_SystemTimeToFileTime(const SYSTEMTIME*,LPFILETIME);
        DWORD boss_fakewin_GetTickCount();
        BOOL boss_fakewin_GetUserObjectInformationW(HANDLE,int,PVOID,DWORD,LPDWORD);
        HLOCAL boss_fakewin_LocalFree(HLOCAL);
        int boss_fakewin_MessageBox(HWND,LPCTSTR,LPCTSTR,UINT);
        BOOL boss_fakewin_CopyFile(LPCSTR,LPCSTR,BOOL);
        BOOL boss_fakewin_DeleteFileA(LPCSTR);
        BOOL boss_fakewin_MoveFileEx(LPCSTR,LPCSTR,DWORD);
        BOOL boss_fakewin_MoveFileExW(LPCWSTR,LPCWSTR,DWORD);
        DWORD boss_fakewin_GetTempPathA(DWORD,LPSTR);
        UINT boss_fakewin_GetTempFileNameA(LPCSTR,LPCSTR,UINT,LPSTR);
        int boss_fakewin_MultiByteToWideChar(UINT,DWORD,LPCSTR,int,LPWSTR,int);
        int boss_fakewin_WideCharToMultiByte(UINT,DWORD,LPCWSTR,int,LPSTR,int,LPCSTR,LPBOOL);
        LONG boss_fakewin_RegCloseKey(HKEY);
        HANDLE boss_fakewin_RegisterEventSource(LPCTSTR,LPCTSTR);
        LONG boss_fakewin_RegOpenKeyExW(HKEY,LPCWSTR,DWORD,REGSAM,PHKEY);
        LONG boss_fakewin_RegQueryValueExW(HKEY,LPCWSTR,LPDWORD,LPDWORD,LPBYTE,LPDWORD);
        BOOL boss_fakewin_ReportEvent(HANDLE,WORD,WORD,DWORD,PSID,WORD,DWORD,LPCTSTR*,LPVOID);
        PVOID boss_fakewin_SecureZeroMemory(PVOID,size_t);
        LRESULT boss_fakewin_SendMessage(HWND,UINT,WPARAM,LPARAM);
        BOOL boss_fakewin_SetFileAttributesW(LPCWSTR,DWORD);
        void boss_fakewin_Sleep(DWORD);
        BOOL boss_fakewin_VerifyVersionInfo(LPOSVERSIONINFOEX,DWORD,DWORDLONG);
        BOOL boss_fakewin_SetEvent(HANDLE);
        BOOL boss_fakewin_ResetEvent(HANDLE);
        HANDLE boss_fakewin_CreateEvent(LPSECURITY_ATTRIBUTES,BOOL,BOOL,LPCTSTR);
        HANDLE boss_fakewin_CreateEventW(LPSECURITY_ATTRIBUTES,BOOL,BOOL,LPCWSTR);
        DWORD boss_fakewin_WaitForSingleObject(HANDLE,DWORD);
        DWORD boss_fakewin_WaitForMultipleObjects(DWORD, const HANDLE*, BOOL, DWORD);
        void boss_fakewin_DeleteCriticalSection(LPCRITICAL_SECTION);
        BOOL boss_fakewin_TryEnterCriticalSection(LPCRITICAL_SECTION);
        void boss_fakewin_EnterCriticalSection(LPCRITICAL_SECTION);
        void boss_fakewin_InitializeCriticalSection(LPCRITICAL_SECTION);
        void boss_fakewin_LeaveCriticalSection(LPCRITICAL_SECTION);
        int boss_fakewin_WSAGetLastError();
        void boss_fakewin_WSASetLastError(int);
        int boss_fakewin_WSACleanup();
        int boss_fakewin_WSAStartup(WORD,LPWSADATA);
        INT boss_fakewin_WSAStringToAddressA(LPSTR,INT,LPWSAPROTOCOL_INFO,LPSOCKADDR,LPINT);
        BOOL boss_fakewin_QueryPerformanceFrequency(LARGE_INTEGER*);
        BOOL boss_fakewin_QueryPerformanceCounter(LARGE_INTEGER*);
        void boss_fakewin_ZeroMemory(PVOID,ULONG_PTR);
        int boss_fakewin_GetObject(HGDIOBJ,int,LPVOID);
        BOOL boss_fakewin_DeleteObject(HGDIOBJ);
        HDC boss_fakewin_GetDC(HWND);
        int boss_fakewin_ReleaseDC(HWND,HDC);
        HBITMAP boss_fakewin_CreateDIBSection(HDC,const BITMAPINFO*,UINT,VOID**,HANDLE,DWORD);
        DWORD boss_fakewin_TlsAlloc(void);
        BOOL boss_fakewin_TlsFree(DWORD dwTlsIndex);
        LPVOID boss_fakewin_TlsGetValue(DWORD dwTlsIndex);
        BOOL boss_fakewin_TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);
        HANDLE boss_fakewin_CreateMutexA(LPSECURITY_ATTRIBUTES,BOOL,LPCSTR);
        BOOL boss_fakewin_ReleaseMutex(HANDLE);
        HANDLE boss_fakewin_CreateSemaphoreA(LPSECURITY_ATTRIBUTES,LONG,LONG,LPCSTR);
        BOOL boss_fakewin_ReleaseSemaphore(HANDLE,LONG,LPLONG);
        void boss_fakewin_GetStartupInfo(LPSTARTUPINFO);
        BOOL boss_fakewin_CreateProcessA(LPCSTR,LPSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,
            BOOL,DWORD,LPVOID,LPCSTR,LPSTARTUPINFO,LPPROCESS_INFORMATION);
    #ifdef __cplusplus
        }
    #endif
#endif

#ifdef __cplusplus
    extern "C" {
#endif
    DWORD boss_fakewin_GetCurrentDirectoryW(DWORD,LPWSTR);
#ifdef __cplusplus
    }
#endif
