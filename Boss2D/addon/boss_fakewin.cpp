#include <boss.h>
#include <iostream>
#include <functional>
#include <cctype>

#include <platform/boss_platform.hpp>

#include "boss_fakewin.h"

extern "C" DWORD boss_fakewin_GetCurrentDirectoryW(DWORD nBufferLength, LPWSTR lpBuffer)
{
    static const WString& Root = Platform::File::GetFullPath((wchars) L"");

    BOSS_ASSERT("nBufferLength가 부족합니다", Root.Length() < (sint32) nBufferLength);
    Memory::Copy(lpBuffer, (wchars) Root, sizeof(wchar_t) * (Root.Length() + 1));
    return Root.Length();
}

#ifdef BOSS_COMMAND_OBJECTS_IS_ENABLED
    #undef socket
    #undef connect
    #undef getsockopt
	#undef setsockopt
    #undef select
    #undef recv
    #undef send
    #undef closesocket
    #undef ioctlsocket
    #undef gethostbyname
    #undef htons
    #undef getaddrinfo
    #undef freeaddrinfo
    #undef gai_strerror
    #undef getpid
    #undef mbstowcs_s
    #undef wcstombs_s
	#undef inet_addr
	#undef bind
	#undef getsockname
	#undef getpeername
	#undef sendto
	#undef recvfrom
	#undef accept
	#undef listen
	#undef getservbyname
	#undef inet_ntoa
	#undef shutdown
	#undef _exit

    #undef CreateFileW
    #undef CreateFileMapping
    #undef CreateDirectory
    #undef RemoveDirectory
    #undef CloseHandle
    #undef MapViewOfFile
    #undef UnmapViewOfFile
    #undef CreateThread
    #undef DeregisterEventSource
    #undef DeviceIoControl
    #undef ExpandEnvironmentStringsA
    #undef ExpandEnvironmentStringsW
    #undef FindClose
    #undef FindFirstFileA
    #undef FindFirstFileW
    #undef FindFirstFileExA
    #undef FindNextFileA
    #undef FindNextFileW
    #undef FindWindow
    #undef FlushConsoleInputBuffer
    #undef FlushFileBuffers
	#undef FormatMessageA	
    #undef FormatMessageW
    #undef GetCurrentDirectoryW
    #undef GetCurrentProcess
    #undef GetCurrentThreadId
    #undef GetExitCodeThread
    #undef GetFileAttributes
    #undef GetFileAttributesW
    #undef GetFileAttributesExA
    #undef GetFileAttributesExW
    #undef GetFullPathNameA
    #undef GetFullPathNameW
    #undef GetLastError
    #undef SetLastError
    #undef LoadLibraryA
    #undef LoadLibraryW
    #undef FreeLibrary
    #undef GetModuleHandleA
    #undef GetModuleHandleW
    #undef GetModuleFileName
    #undef GetProcAddress
    #undef GetProcessAffinityMask
    #undef GetProcessWindowStation
    #undef GetProcessTimes
    #undef GetShortPathNameW
    #undef GetStdHandle
    #undef GetSystemInfo
    #undef GetNativeSystemInfo
    #undef GetSystemTimeAsFileTime
    #undef SystemTimeToFileTime
    #undef GetTickCount
    #undef GetUserObjectInformationW
    #undef LocalFree
    #undef MessageBox
    #undef CopyFile
    #undef DeleteFileA
    #undef MoveFileEx
    #undef MoveFileExW
    #undef GetTempPathA
    #undef GetTempFileNameA
    #undef MultiByteToWideChar
    #undef WideCharToMultiByte
    #undef RegCloseKey
    #undef RegisterEventSource
    #undef RegOpenKeyExW
    #undef RegQueryValueExW
    #undef ReportEvent
    #undef SecureZeroMemory
    #undef SendMessage
    #undef SetFileAttributesW
    #undef Sleep
    #undef SleepEx
    #undef VerifyVersionInfo
    #undef SetEvent
    #undef ResetEvent
    #undef CreateEvent
    #undef CreateEventW
    #undef WaitForSingleObject
    #undef WaitForMultipleObjects
    #undef DeleteCriticalSection
    #undef TryEnterCriticalSection
    #undef EnterCriticalSection
    #undef InitializeCriticalSection
    #undef LeaveCriticalSection
    #undef WSAGetLastError
    #undef WSASetLastError
    #undef WSACleanup
    #undef WSAStartup
    #undef WSAStringToAddressA
    #undef QueryPerformanceFrequency
    #undef QueryPerformanceCounter
    #undef ZeroMemory
    #undef GetObject
    #undef DeleteObject
    #undef GetDC
    #undef ReleaseDC
    #undef CreateDIBSection
    #undef TlsAlloc
    #undef TlsFree
    #undef TlsGetValue
    #undef TlsSetValue
    #undef CreateMutexA
    #undef ReleaseMutex
    #undef CreateSemaphoreA
    #undef ReleaseSemaphore
    #undef GetStartupInfo
    #undef CreateProcessA
    #undef GetEnvironmentStrings
    #undef GetEnvironmentStringsW
    #undef GetSystemDirectoryA
    #undef GetSystemDirectoryW
    #undef GetEnvironmentVariableA
    #undef GetEnvironmentVariableW
    #undef GetSystemTime
    #undef SwitchToFiber
    #undef DeleteFiber
    #undef ConvertFiberToThread
    #undef ConvertThreadToFiber
    #undef CreateFiberEx
    #undef ConvertThreadToFiberEx
    #undef CreateFiber
	#undef GetConsoleMode
	#undef SetConsoleMode
	#undef ReadFile
	#undef ReadFileEx
	#undef GetFileType
	#undef PeekNamedPipe
	#undef strerror_s
	#undef ntohs
	#undef timeGetTime
	#undef GetFiberData
	#undef GetCurrentFiber
	#undef GetVersionExA
	#undef GetVersionExW
	#undef WSAIoctl
	#undef err2stringW
	#undef err2stringA
	#undef ldap_get_option
	#undef ldap_get_optionW
	#undef ldap_set_option
	#undef ldap_set_optionW
	#undef ldap_initW
	#undef ldap_init
	#undef ldap_simple_bindW
	#undef ldap_simple_bindA
	#undef ldap_simple_bind_sW
	#undef ldap_simple_bind_sA
	#undef ldap_searchW
	#undef ldap_searchA
	#undef ldap_search_sW
	#undef ldap_search_sA
	#undef ldap_first_entry
	#undef ldap_get_dnW
	#undef ldap_get_dn	
	#undef ldap_first_attributeW
	#undef ldap_first_attribute
	#undef ldap_next_attributeW
	#undef ldap_next_attribute
	#undef ldap_get_values_lenW
	#undef ldap_get_values_len
	#undef ldap_value_free_len
	#undef ldap_next_entry
	#undef ldap_msgfree
	#undef ldap_unbind
	#undef ldap_unbind_s
		
    #undef _waccess
	#undef _chmod
    #undef _wchmod
    #undef _wfopen
    #undef _wfopen_s
    #undef _fopen
    #undef _fopen_s
    #undef _fseek
    #undef _ftell
    #undef _rewind
    #undef _fread
    #undef _fwrite
    #undef _fgetc
    #undef _fgets
    #undef _ungetc
    #undef _fclose
    #undef _wopen
    #undef _close
    #undef _read
    #undef _write
    #undef _get_osfhandle
    #undef _lseek
    #undef _lseeki64
    #undef _lfind
    #undef _chsize_s
    #undef _stat
    #undef _stat64
    #undef _fstat
    #undef _fstat64
    #undef _wchdir
    #undef _wmkdir
    #undef _wrmdir
    #undef _wunlink
    #undef _mktemp_s
    #undef _wgetenv
    #undef _wgetcwd
    #undef fd_set
    #undef FD_SET
    #undef FD_ZERO
    #undef _alloca
    #undef _fileno
    #undef _getch
    #undef _getdrive
    #undef _lrotl
	#undef _lrotr
    #undef _rotl
    #undef _setmode
    #undef stricmp
    #undef _stricmp
    #undef strnicmp
    #undef _strnicmp
    #undef _mbsicmp
    #undef _wcsicmp
    #undef _snprintf
    #undef _vscprintf
    #undef _vsnprintf_s
    #undef _vsntprintf
    #undef _vsnwprintf
    #undef _beginthreadex
    #undef _fullpath
    #undef _ltoa
    #undef _ultoa
    #undef _isnan
    #undef _finite
    #undef _splitpath
    #undef _strtoi64
    #undef _strtoui64
    #undef _set_errno
	#undef _byteswap_ushort
	#undef _byteswap_ulong
	#undef _byteswap_uint64
	#undef strlen
	#undef wcslen
	#undef strerror
	#undef strerror_s	
	#undef strcpy_s
	#undef wcscpy_s
	#undef strcpy
	#undef wcscpy
	#undef _strdup
	#undef _wcsdup
	#undef strpbrk
	#undef wcspbrk
	#undef ber_free	
	
    // 명칭재정의관련
    #undef wfopen
    #undef wfopen_s
    #undef fopen
    #undef fopen_s
    #undef fseek
    #undef ftell
    #undef rewind
    #undef fread
    #undef fwrite
    #undef fgetc
    #undef fgets
    #undef ungetc
    #undef fclose
    #undef ltoa
    #undef _snprintf

    extern "C" SOCKET boss_fakewin_socket(int af, int type, int protocol)
    {
        BOSS_ASSERT("af는 AF_INET만 지원합니다", af == AF_INET);
        BOSS_ASSERT("type는 SOCK_STREAM만 지원합니다", type == SOCK_STREAM);
        BOSS_ASSERT("protocol는 (IPPROTO_IP|IPPROTO_TCP)만 지원합니다",
            protocol == IPPROTO_IP || protocol == IPPROTO_TCP);

        id_socket_read SocketID = Platform::Socket::OpenForTcp();

        SOCKET NewSocket;
        Memory::Copy(&NewSocket, &SocketID, sizeof(SOCKET));
        return NewSocket;
    }

    extern "C" int boss_fakewin_connect(SOCKET s, const struct sockaddr* name, int namelen)
    {
        const struct sockaddr_in* info = (const struct sockaddr_in*) name;
        String Domain = String::Format("%d.%d.%d.%d",
            info->sin_addr.S_un.S_un_b.s_b4, info->sin_addr.S_un.S_un_b.s_b3,
            info->sin_addr.S_un.S_un_b.s_b2, info->sin_addr.S_un.S_un_b.s_b1);
        const bool Result = Platform::Socket::Connect((id_socket) s, Domain, info->sin_port);
        return (Result)? 0 : SOCKET_ERROR;
    }

    extern "C" int boss_fakewin_getsockopt(SOCKET s, int level, int optname, char* optval, int* optlen)
    {
        BOSS_ASSERT("level는 SOL_SOCKET만 지원합니다", level == SOL_SOCKET);
        BOSS_ASSERT("optname는 SO_ERROR만 지원합니다", optname == SO_ERROR);
        return 0;
    }
	
    extern "C" int boss_fakewin_setsockopt(SOCKET s, int level, int optname, const char* optval, int* optlen)
    {
        BOSS_ASSERT("level는 SOL_SOCKET만 지원합니다", level == SOL_SOCKET);
        BOSS_ASSERT("optname는 SO_ERROR만 지원합니다", optname == SO_ERROR);
        return 0;
    }

    extern "C" int boss_fakewin_select(int nfds, boss_fakewin_struct_fd_set* readfds, boss_fakewin_struct_fd_set* writefds, boss_fakewin_struct_fd_set* exceptfds, const struct timeval* timeout)
    {
        sint32 Result = 0;

        if(readfds)
        for(sint32 i = 0, iend = readfds->fd_count; i < iend; ++i)
            Result += !!Platform::Socket::RecvAvailable((id_socket) readfds->fd_array[i], 1);
        
        if(writefds)
        for(sint32 i = 0, iend = writefds->fd_count; i < iend; ++i)
        {
            BOSS_ASSERT("boss_fakewin_select writefds!!!", false);
        }
        
        if(exceptfds)
        for(sint32 i = 0, iend = exceptfds->fd_count; i < iend; ++i)
        {
            BOSS_ASSERT("boss_fakewin_select writefds!!!", false);
        }
        return Result;
    }

    extern "C" int boss_fakewin_recv(SOCKET s, char* buf, int len, int flags)
    {
        BOSS_ASSERT("flags는 0만 지원합니다", flags == 0);

        const int Result = Platform::Socket::Recv((id_socket) s, (uint08*) buf, len);
        if(Result < 0) boss_fakewin_WSASetLastError(-Result);
        return Result;
    }

    extern "C" int boss_fakewin_send(SOCKET s, const char* buf, int len, int flags)
    {
        BOSS_ASSERT("flags는 0만 지원합니다", flags == 0);

        return Platform::Socket::Send((id_socket) s, (bytes) buf, len);
    }

    extern "C" int boss_fakewin_closesocket(SOCKET s)
    {
        const bool Result = Platform::Socket::Close((id_socket) s);
        return (Result)? 0 : SOCKET_ERROR;
    }

    extern "C" int boss_fakewin_ioctlsocket(SOCKET s, long cmd, u_long* argp)
    {BOSS_ASSERT("ioctlsocket준비중", false); return 0;}
    extern "C" struct hostent* boss_fakewin_gethostbyname(const char* name)
    {BOSS_ASSERT("gethostbyname준비중", false); return 0;}
    extern "C" unsigned short boss_fakewin_htons(unsigned short number)
    {BOSS_ASSERT("htons준비중", false); return 0;}

    extern "C" int boss_fakewin_getaddrinfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult)
    {
        struct addrinfo *ainfo, *ai;
        int p = 0;

        if ((ainfo = (struct addrinfo*) Memory::Alloc(sizeof(struct addrinfo))) == nullptr)
            return -1;

        if ((ainfo->ai_hostent = (struct hostent*) Platform::Socket::GetHostByName(pNodeName)) == nullptr) {
            BOSS_ASSERT("호스트를 찾을 수 없습니다", false);
            Memory::Free(ainfo);
            return -2;
        }

        ainfo->ai_servent = (struct servent*) Platform::Socket::GetServByName(pServiceName, 0);

        if (ainfo->ai_servent)
            ainfo->ai_port = ainfo->ai_servent->s_port;
        else
            ainfo->ai_port = Parser(pServiceName).ReadInt();

        Memory::Copy(&ainfo->ai_addr_in.sin_addr,
                ainfo->ai_hostent->h_addr_list[0],
                ainfo->ai_hostent->h_length);

        ainfo->ai_protocol = 0;
        ainfo->ai_socktype = pHints->ai_socktype;
        ainfo->ai_family = ainfo->ai_hostent->h_addrtype;
        ainfo->ai_addr_in.sin_family = ainfo->ai_family;
        ainfo->ai_addr_in.sin_port = ainfo->ai_port;
        ainfo->ai_addr = (struct sockaddr *) &ainfo->ai_addr_in;
        ainfo->ai_addrlen = sizeof(struct sockaddr_in);

        *ppResult = ainfo;

        if (ainfo->ai_hostent->h_addr_list[1] == nullptr) {
            ainfo->ai_next = nullptr;
            return 0;
        }

        ai = ainfo;

        for (p = 1; ainfo->ai_hostent->h_addr_list[p] != nullptr; p++) {
            ai->ai_next = (struct addrinfo*) Memory::Alloc(sizeof(struct addrinfo));
            Memory::Copy(ai->ai_next, ainfo, sizeof(struct addrinfo));
            Memory::Copy(&ai->ai_next->ai_addr_in.sin_addr,
                ainfo->ai_hostent->h_addr_list[p],
                ainfo->ai_hostent->h_length);
            ai->ai_next->ai_addr = (struct sockaddr *) &ai->ai_next->ai_addr_in;
            ai = ai->ai_next;
        }

        ai->ai_next = nullptr;
        return 0;
    }

    extern "C" void boss_fakewin_freeaddrinfo(struct addrinfo* ai)
    {
        struct addrinfo *p, *next;

        p = ai;

        while(p != nullptr) {
            next = p->ai_next;
            Memory::Free(p);
            p = next;
        }
    }

    extern "C" const char* boss_fakewin_gai_strerror(int ret)
    {
        switch(ret)
        {
        case -1: return "Out of memory";
        case -2: return "Address lookup failed";
        }
        return "Unknown error";
    }

    extern "C" int boss_fakewin_getpid()
    {
        BOSS_ASSERT("getpid준비중", false); return 0;
    }

    extern "C" errno_t boss_fakewin_mbstowcs_s(size_t* pReturnValue, wchar_t* wcstr,
        size_t sizeInWords, const char* mbstr, size_t count)
    {
        const WString& NewString = WString::FromChars(mbstr, count);
        const sint32 CopyLen = Math::Min(NewString.Length(), sizeInWords - 1);
        if(pReturnValue) *pReturnValue = CopyLen;
        if(wcstr)
        {
            Memory::Copy(wcstr, (wchars) NewString, sizeof(wchar_t) * CopyLen);
            wcstr[CopyLen] = L'\0';
        }
        return 0;
    }

    extern "C" errno_t boss_fakewin_wcstombs_s(size_t* pReturnValue, char* mbstr,
        size_t sizeInBytes, const wchar_t* wcstr, size_t count)
    {
        const String& NewString = String::FromWChars(wcstr, count);
        const sint32 CopyLen = Math::Min(NewString.Length(), sizeInBytes - 1);
        if(pReturnValue) *pReturnValue = CopyLen;
        if(mbstr)
        {
            Memory::Copy(mbstr, (chars) NewString, sizeof(char) * CopyLen);
            mbstr[CopyLen] = '\0';
        }
        return 0;
    }
	
	extern "C" unsigned long boss_fakewin_inet_addr(const char *cp)
	{
		BOSS_ASSERT("inet_addr준비중", false);
		return 0;
	}
	extern "C" int boss_fakewin_bind(SOCKET s, const struct sockaddr* addr, int namelen)
	{
		BOSS_ASSERT("bind준비중", false);
		return 0;
	}
	extern "C" int boss_fakewin_getsockname(SOCKET s, const struct sockaddr* addr, int* namelen)
	{
		BOSS_ASSERT("getsockname준비중", false);
		return 0;
	}	
	extern "C" int boss_fakewin_getpeername(SOCKET s, const struct sockaddr* addr, int* namelen)
	{
		BOSS_ASSERT("getpeername준비중", false);
		return 0;
	}
    extern "C" int boss_fakewin_sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
	{
		BOSS_ASSERT("sendto준비중", false);
		return 0;
	}
    extern "C" int boss_fakewin_recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* to, int* fromlen)
    {
        BOSS_ASSERT("recvfrom준비중", false);
        return 0;
    }
    extern "C" SOCKET boss_fakewin_accept(SOCKET s, struct sockaddr* addr, int* addrlen)
    {
        BOSS_ASSERT("accept준비중", false);
        return 0;
    }
    extern "C" int boss_fakewin_listen(SOCKET s, int backlog)
    {
        BOSS_ASSERT("listen준비중", false);
        return 0;
    }
	extern "C" struct servent* boss_fakewin_getservbyname(const char* name, const char* proto)
    {
        BOSS_ASSERT("getservbyname준비중", false);
        return 0;
    }
	extern "C" char* boss_fakewin_inet_ntoa(struct in_addr in)
    {
        BOSS_ASSERT("inet_ntoa준비중", false);
        return 0;
    }
	extern "C" int boss_fakewin_shutdown(SOCKET s, int how)
	{
		BOSS_ASSERT("shutdown준비중", false);
        return 0;
	}
	extern "C" void boss_fakewin_exit(int status)
	{
		BOSS_ASSERT("_exit준비중", false);
	}	
	
    class Helper_MapInfo
    {
    public:
        Helper_MapInfo() {fd = -1; readonly = false;}
        ~Helper_MapInfo() {}

    public:
        sint32 fd;
        bool readonly;
    };
    sint32 _LastMapInfoID = 0;
    Map<Helper_MapInfo> _FDToMapInfo;

    extern "C" HANDLE boss_fakewin_CreateFileW(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE)
    {
        BOSS_ASSERT("CreateFileW준비중", false); return 0;
    }

    extern "C" HANDLE boss_fakewin_CreateFileMapping(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect,
        DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCTSTR lpName)
    {
        BOSS_ASSERT("lpAttributes는 nullptr만 지원합니다", lpAttributes == nullptr);

        const sint32 CurFD = *((sint32*) &hFile);
        Platform::File::FDOpenRetain(CurFD);

        Helper_MapInfo& NewMapInfo = _FDToMapInfo[++_LastMapInfoID];
        NewMapInfo.fd = CurFD;
        NewMapInfo.readonly = !!(flProtect & PAGE_READONLY);
        return *((HANDLE*) &_LastMapInfoID);
    }

    extern "C" BOOL boss_fakewin_CreateDirectory(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
    {
        BOSS_ASSERT("CreateDirectory준비중", false); return 0;
    }

    extern "C" BOOL boss_fakewin_RemoveDirectory(LPCSTR lpPathName)
    {
        BOSS_ASSERT("RemoveDirectory준비중", false); return 0;
    }

    extern "C" BOOL boss_fakewin_CloseHandle(HANDLE hObject)
    {
        uint64 OldFileInfoID = PtrToUint64(hObject);
        if(Helper_MapInfo* OldMapInfo = _FDToMapInfo.Access(OldFileInfoID))
        {
            Platform::File::FDClose(OldMapInfo->fd);
            _FDToMapInfo.Remove(OldFileInfoID);
        }
        return true;
    }

    extern "C" LPVOID boss_fakewin_MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh,
        DWORD dwFileOffsetLow, size_t dwNumberOfBytesToMap)
    {
        uint64 CurFileInfoID = PtrToUint64(hFileMappingObject);
        if(Helper_MapInfo* CurFileInfo = _FDToMapInfo.Access(CurFileInfoID))
        {
            const sint64 Offset64 = (((sint64) dwFileOffsetHigh) << 32) | dwFileOffsetLow;
            return Platform::File::FDMap(CurFileInfo->fd, Offset64, dwNumberOfBytesToMap, CurFileInfo->readonly);
        }
        return nullptr;
    }

    extern "C" BOOL boss_fakewin_UnmapViewOfFile(LPCVOID lpBaseAddress)
    {
        return Platform::File::FDUnmap(lpBaseAddress);
    }

    extern "C" HANDLE boss_fakewin_CreateThread(LPSECURITY_ATTRIBUTES,size_t,LPTHREAD_START_ROUTINE,LPVOID,DWORD,LPDWORD) {BOSS_ASSERT("CreateThread준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_DeregisterEventSource(HANDLE) {BOSS_ASSERT("DeregisterEventSource준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_DeviceIoControl(HANDLE,DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD,LPOVERLAPPED) {BOSS_ASSERT("DeviceIoControl준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_ExpandEnvironmentStringsA(LPCSTR lpSrc, LPSTR lpDst, DWORD nSize)
    {
		BOSS_ASSERT("ExpandEnvironmentStringsA동작 확인해야 함", false);
        static const WString& wRoot = Platform::File::GetFullPath((wchars) L"");
        const String Root = String::FromWChars(wRoot);

        String Result;
        bool IsInnerCode = false;
        while(*lpSrc)
        {
            if(*lpSrc != '%')
            {
                Result += *lpSrc;
                lpSrc += 1;
            }
            else if(IsInnerCode)
            {
                IsInnerCode = false;
                Result += *lpSrc;
                lpSrc += 1;
            }
            else if(!Memory::Compare("%HOMEDRIVE%", lpSrc, sizeof(char) * 11))
            {
                Result += Root[0];
                Result += Root[1];
                lpSrc += 11;
            }
            else if(!Memory::Compare("%HOMEPATH%", lpSrc, sizeof(char) * 10))
            {
                Result += &((chars) Root)[2];
                lpSrc += 10;
            }
            else if(!Memory::Compare("%USERPROFILE%", lpSrc, sizeof(char) * 13))
            {
                Result += Root;
                lpSrc += 13;
            }
            else if(!Memory::Compare("%APPDATA%", lpSrc, sizeof(char) * 9))
            {
                Result += Root;
                Result += (chars) "/appdata/roaming";
                lpSrc += 9;
            }
            else if(!Memory::Compare("%LOCALAPPDATA%", lpSrc, sizeof(char) * 14))
            {
                Result += Root;
                Result += (chars) "/appdata/local";
                lpSrc += 14;
            }
            else
            {
                IsInnerCode = true;
                Result += *lpSrc;
                lpSrc += 1;
            }
        }

        BOSS_ASSERT("nSize가 부족합니다", Result.Length() < (sint32) nSize);
        Memory::Copy(lpDst, (chars) Result, sizeof(char) * (Result.Length() + 1));
        return Result.Length();
    }

    extern "C" DWORD boss_fakewin_ExpandEnvironmentStringsW(LPCWSTR lpSrc, LPWSTR lpDst, DWORD nSize)
    {
        static const WString& Root = Platform::File::GetFullPath((wchars) L"");

        WString Result;
        bool IsInnerCode = false;
        while(*lpSrc)
        {
            if(*lpSrc != L'%')
            {
                Result += *lpSrc;
                lpSrc += 1;
            }
            else if(IsInnerCode)
            {
                IsInnerCode = false;
                Result += *lpSrc;
                lpSrc += 1;
            }
            else if(!Memory::Compare(L"%HOMEDRIVE%", lpSrc, sizeof(wchar_t) * 11))
            {
                Result += Root[0];
                Result += Root[1];
                lpSrc += 11;
            }
            else if(!Memory::Compare(L"%HOMEPATH%", lpSrc, sizeof(wchar_t) * 10))
            {
                Result += &((wchars) Root)[2];
                lpSrc += 10;
            }
            else if(!Memory::Compare(L"%USERPROFILE%", lpSrc, sizeof(wchar_t) * 13))
            {
                Result += Root;
                lpSrc += 13;
            }
            else if(!Memory::Compare(L"%APPDATA%", lpSrc, sizeof(wchar_t) * 9))
            {
                Result += Root;
                Result += (wchars) L"/appdata/roaming";
                lpSrc += 9;
            }
            else if(!Memory::Compare(L"%LOCALAPPDATA%", lpSrc, sizeof(wchar_t) * 14))
            {
                Result += Root;
                Result += (wchars) L"/appdata/local";
                lpSrc += 14;
            }
            else
            {
                IsInnerCode = true;
                Result += *lpSrc;
                lpSrc += 1;
            }
        }

        BOSS_ASSERT("nSize가 부족합니다", Result.Length() < (sint32) nSize);
        Memory::Copy(lpDst, (wchars) Result, sizeof(wchar_t) * (Result.Length() + 1));
        return Result.Length();
    }

    class SearchResult
    {
    public:
        SearchResult(chars pathname)
        {
            String Checkname = pathname;
            if(Checkname[-2] == '*') Checkname.Sub(1);
            if(Checkname[-2] != '/' && Checkname[-2] != '\\')
                Checkname.Add("/");
            Pathname = WString::FromChars(Checkname);
            Cursor = 0;
        }
        ~SearchResult() {}

        void GetResultAtCursor(LPWIN32_FIND_DATAW lpFindFileData)
        {
            const WString& Filename = WString::FromChars(Itemnames[Cursor]);
            WIN32_FILE_ATTRIBUTE_DATA FileData;
            BOOL Result = boss_fakewin_GetFileAttributesExW(Pathname + Filename, GetFileExInfoStandard, &FileData);

            Memory::Set(lpFindFileData, 0x00, sizeof(WIN32_FIND_DATAW));
            Memory::Copy(lpFindFileData->cFileName, (wchars) Filename, sizeof(wchar_t) * (Filename.Length() + 1));
            if(Result) // ".", ".."의 경우 실패할 수 있음(WIN32에서만 성공함)
            {
                lpFindFileData->dwFileAttributes = FileData.dwFileAttributes;
                lpFindFileData->ftCreationTime = FileData.ftCreationTime;
                lpFindFileData->ftLastAccessTime = FileData.ftLastAccessTime;
                lpFindFileData->ftLastWriteTime = FileData.ftLastWriteTime;
                lpFindFileData->nFileSizeHigh = FileData.nFileSizeHigh;
                lpFindFileData->nFileSizeLow = FileData.nFileSizeLow;
            }
        }

    public:
        WString Pathname;
        Strings Itemnames;
        sint32 Cursor;
    };

    extern "C" BOOL boss_fakewin_FindClose(HANDLE hFindFile)
    {
        return (boss_closedir((void*) hFindFile) == 0);

        /*SearchResult* CurResult = (SearchResult*) hFindFile;
        delete CurResult;
        return true;*/
    }

    extern "C" HANDLE boss_fakewin_FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
    {
        BOSS_ASSERT("FindFirstFileA준비중", false);
        return 0;
    }

    extern "C" HANDLE boss_fakewin_FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
    {
        HANDLE NewHandle = (HANDLE) boss_opendir(String::FromWChars(lpFileName));
        return (NewHandle)? NewHandle : INVALID_HANDLE_VALUE;

        /*const String FilenameUTF8 = String::FromWChars(lpFileName);
        SearchResult* NewSearchResult = new SearchResult(FilenameUTF8);
        NewSearchResult->Itemnames.AtAdding() = ".";
        NewSearchResult->Itemnames.AtAdding() = "..";

        sint32 Result = Platform::File::Search(FilenameUTF8,
            [](chars itemname, payload data) -> void
            {
                SearchResult* NewSearchResult = (SearchResult*) data;
                NewSearchResult->Itemnames.AtAdding() = itemname;
            }, NewSearchResult, false);

        if(Result == -1)
        {
            delete NewSearchResult;
            return INVALID_HANDLE_VALUE;
        }

        NewSearchResult->GetResultAtCursor(lpFindFileData);
        NewSearchResult->Cursor++;
        return (HANDLE) NewSearchResult;*/
    }

    extern "C" HANDLE boss_fakewin_FindFirstFileExA(LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId,
        LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
    {
        BOSS_ASSERT("FindFirstFileA준비중", false);
        return 0;
    }

    extern "C" BOOL boss_fakewin_FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
    {
        BOSS_ASSERT("FindNextFileA준비중", false);
        return 0;
    }

    extern "C" BOOL boss_fakewin_FindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
    {
        void* curdir = boss_readdir((void*) hFindFile);
        chars CurDirname = boss_getdirname(curdir);
        WString CurDirnameW = WString::FromChars(CurDirname);
        Memory::Copy(lpFindFileData->cFileName, (wchars) CurDirnameW, CurDirnameW.Length());
        lpFindFileData->cFileName[CurDirnameW.Length()] = L'\0';
        return TRUE;

        /*SearchResult* CurResult = (SearchResult*) hFindFile;
        if(CurResult->Itemnames.Count() <= CurResult->Cursor)
        {
            boss_fakewin_SetLastError(ERROR_NO_MORE_FILES);
            return false;
        }

        CurResult->GetResultAtCursor(lpFindFileData);
        CurResult->Cursor++;
        return true;*/
    }

    extern "C" HWND boss_fakewin_FindWindow(LPCTSTR,LPCTSTR) {BOSS_ASSERT("FindWindow준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_FlushConsoleInputBuffer(HANDLE) {BOSS_ASSERT("FlushConsoleInputBuffer준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_FlushFileBuffers(HANDLE) {BOSS_ASSERT("FlushFileBuffers준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_FormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId,
        LPSTR lpBuffer, DWORD nSize, va_list* Arguments)
    {
		BOSS_ASSERT("FormatMessageA동작 확인해야 됨", false);
        *((LPSTR*) lpBuffer) = (LPSTR) Buffer::Alloc<char>(BOSS_DBG 13 + 1);
        Memory::Copy(*((LPSTR*) lpBuffer), "Unknown Error", sizeof(char) * (13 + 1));
        return 13;
    }
    extern "C" DWORD boss_fakewin_FormatMessageW(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId,
        LPWSTR lpBuffer, DWORD nSize, va_list* Arguments)
    {
        *((LPWSTR*) lpBuffer) = (LPWSTR) Buffer::Alloc<wchar_t>(BOSS_DBG 13 + 1);
        Memory::Copy(*((LPWSTR*) lpBuffer), L"Unknown Error", sizeof(wchar_t) * (13 + 1));
        return 13;
    }

    extern "C" HANDLE boss_fakewin_GetCurrentProcess() {BOSS_ASSERT("GetCurrentProcess준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_GetCurrentThreadId()
    {
        return (Platform::Utility::CurrentThreadID() & 0xFFFFFFFF);
    }

    extern "C" BOOL boss_fakewin_GetExitCodeThread(HANDLE,LPDWORD) {BOSS_ASSERT("GetExitCodeThread준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_GetFileAttributes(LPCSTR lpFileName) {BOSS_ASSERT("GetFileAttributes준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_GetFileAttributesW(LPCWSTR lpFileName)
    {
        const DWORD FileAttributes = Platform::File::GetAttributes(lpFileName);
        if(FileAttributes == -1)
            boss_fakewin_SetLastError(ERROR_FILE_NOT_FOUND);
        return FileAttributes;
    }

    extern "C" BOOL boss_fakewin_GetFileAttributesExA(LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
    {
        BOSS_ASSERT("GetFileAttributesExA준비중", false);
        return 0;
    }

    extern "C" BOOL boss_fakewin_GetFileAttributesExW(LPCWSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
    {
        BOSS_ASSERT("fInfoLevelId는 GetFileExInfoStandard만 지원합니다", fInfoLevelId == GetFileExInfoStandard);

        uint64 GetSize, GetCreateTime, GetAccessTime, GetModifyTime;
        const DWORD FileAttributes = Platform::File::GetAttributes(lpFileName,
            &GetSize, &GetCreateTime, &GetAccessTime, &GetModifyTime);
        if(FileAttributes == -1)
        {
            boss_fakewin_SetLastError(ERROR_FILE_NOT_FOUND);
            return false;
        }

        LPWIN32_FILE_ATTRIBUTE_DATA Result = (LPWIN32_FILE_ATTRIBUTE_DATA) lpFileInformation;
        Result->dwFileAttributes = FileAttributes;
        Result->nFileSizeHigh = GetSize >> 32;
        Result->nFileSizeLow = GetSize & 0xFFFFFFFF;
        Result->ftCreationTime.dwHighDateTime = GetCreateTime >> 32;
        Result->ftCreationTime.dwLowDateTime = GetCreateTime & 0xFFFFFFFF;
        Result->ftLastAccessTime.dwHighDateTime = GetAccessTime >> 32;
        Result->ftLastAccessTime.dwLowDateTime = GetAccessTime & 0xFFFFFFFF;
        Result->ftLastWriteTime.dwHighDateTime = GetModifyTime >> 32;
        Result->ftLastWriteTime.dwLowDateTime = GetModifyTime & 0xFFFFFFFF;
        return true;
    }

    extern "C" DWORD boss_fakewin_GetFullPathNameA(LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR* lpFilePart)
    {
        BOSS_ASSERT("lpFilePart는 nullptr만 지원합니다", lpFilePart == nullptr);

        const WString FileNameUTF16 = WString::FromChars(lpFileName);
        const String NewFullPath = String::FromWChars(Platform::File::GetFullPath(FileNameUTF16));
        if(!lpBuffer)
            return NewFullPath.Length() + 1;

        const sint32 CopyLength = Math::Min(NewFullPath.Length(), nBufferLength - 1);
        Memory::Copy(lpBuffer, (chars) NewFullPath, sizeof(char) * CopyLength);
        lpBuffer[CopyLength] = '\0';
        return CopyLength;
    }

    extern "C" DWORD boss_fakewin_GetFullPathNameW(LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR* lpFilePart)
    {
        BOSS_ASSERT("lpFilePart는 nullptr만 지원합니다", lpFilePart == nullptr);

        const WString NewFullPath = Platform::File::GetFullPath(lpFileName);
        if(!lpBuffer)
            return NewFullPath.Length() + 1;

        const sint32 CopyLength = Math::Min(NewFullPath.Length(), nBufferLength - 1);
        Memory::Copy(lpBuffer, (wchars) NewFullPath, sizeof(wchar_t) * CopyLength);
        lpBuffer[CopyLength] = L'\0';
        return CopyLength;
    }

    static DWORD Common_ErrCode = 0;
    extern "C" DWORD boss_fakewin_GetLastError()
    {return Common_ErrCode;}
    extern "C" void boss_fakewin_SetLastError(DWORD dwErrCode)
    {Common_ErrCode = dwErrCode;}

    extern "C" HMODULE boss_fakewin_LoadLibraryA(LPCSTR lpFileName) {BOSS_ASSERT("LoadLibraryA준비중", false); return 0;}
    extern "C" HMODULE boss_fakewin_LoadLibraryW(LPCWSTR lpFileName) {BOSS_ASSERT("LoadLibraryW준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_FreeLibrary(HMODULE hModule) {BOSS_ASSERT("FreeLibrary준비중", false); return 0; }

    extern "C" HMODULE boss_fakewin_GetModuleHandleA(LPCSTR)
    {
        return 0;
    }

    extern "C" HMODULE boss_fakewin_GetModuleHandleW(LPCWSTR) {BOSS_ASSERT("GetModuleHandleW준비중", false); return 0;}
    extern "C" DWORD boss_fakewin_GetModuleFileName(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
    {
        const String AppPath = "myapp.exe";
        String AppPath2 = String::FromWChars(Platform::File::GetFullPath(WString::FromChars(AppPath)));
        AppPath2.Replace('/', '\\');
        Memory::Copy(lpFilename, (chars) AppPath2, AppPath2.Length() + 1);
        return AppPath2.Length();
    }
    extern "C" FARPROC boss_fakewin_GetProcAddress(HMODULE,LPCSTR) {BOSS_ASSERT("GetProcAddress준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_GetProcessAffinityMask(HANDLE,PDWORD_PTR,PDWORD_PTR) {BOSS_ASSERT("GetProcessAffinityMask준비중", false); return 0;}

    extern "C" HWINSTA boss_fakewin_GetProcessWindowStation()
    {
        return (HWINSTA) 0x0000005c;
    }

    extern "C" BOOL boss_fakewin_GetProcessTimes(HANDLE hProcess, LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime)
    {
        BOSS_ASSERT("GetProcessTimes준비중", false); return 0;
    }

    extern "C" DWORD boss_fakewin_GetShortPathNameW(LPCWSTR lpszLongPath, LPWSTR lpszShortPath, DWORD cchBuffer)
    {
        const WString& NewString = (wchars) lpszLongPath;
        Memory::Copy(lpszShortPath, (wchars) NewString, sizeof(wchar_t) * (NewString.Length() + 1));
        return NewString.Length();
    }

    extern "C" HANDLE boss_fakewin_GetStdHandle(DWORD) {BOSS_ASSERT("GetStdHandle준비중", false); return 0;}

    extern "C" void boss_fakewin_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
    {
        Memory::Set(lpSystemInfo, 0x00, sizeof(SYSTEM_INFO));
        lpSystemInfo->dwNumberOfProcessors = 1;
        lpSystemInfo->dwAllocationGranularity = 65536;
    }

    extern "C" void boss_fakewin_GetNativeSystemInfo(LPSYSTEM_INFO lpSystemInfo)
    {
        Memory::Set(lpSystemInfo, 0x00, sizeof(SYSTEM_INFO));
        lpSystemInfo->dwNumberOfProcessors = 1;
        lpSystemInfo->dwAllocationGranularity = 65536;
    }

    extern "C" void boss_fakewin_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
    {
        uint64 FileTime = 10 * 1000 * Platform::Utility::CurrentTimeMsec();
        lpSystemTimeAsFileTime->dwHighDateTime = FileTime >> 32;
        lpSystemTimeAsFileTime->dwLowDateTime = FileTime & 0xFFFFFFFF;
    }

    extern "C" BOOL boss_fakewin_SystemTimeToFileTime(const SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime)
    {
        BOSS_ASSERT("SystemTimeToFileTime준비중", false);
        return 0;
    }

    extern "C" DWORD boss_fakewin_GetTickCount()
    {
        return (DWORD) Platform::Utility::CurrentTimeMsec();
    }

    extern "C" BOOL boss_fakewin_GetUserObjectInformationW(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded)
    {
        BOSS_ASSERT("hObj는 0x0000005c만 지원합니다", hObj == (HANDLE) 0x0000005c);
        BOSS_ASSERT("nIndex는 UOI_NAME만 지원합니다", nIndex == UOI_NAME);

        if(lpnLengthNeeded)
            *lpnLengthNeeded = sizeof(wchar_t) * (7 + 1);

        if(pvInfo)
        {
            Memory::Copy(pvInfo, L"WinSta0", sizeof(wchar_t) * (7 + 1));
            return true;
        }

        boss_fakewin_SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return false;
    }

    extern "C" HLOCAL boss_fakewin_LocalFree(HLOCAL hMem)
    {
        Buffer::Free((buffer) hMem);
        return nullptr;
    }

    extern "C" int boss_fakewin_MessageBox(HWND,LPCTSTR,LPCTSTR,UINT) {BOSS_ASSERT("MessageBox준비중", false); return 0;}

    extern "C" BOOL boss_fakewin_CopyFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
    {BOSS_ASSERT("CopyFile준비중", false); return 0;}

    extern "C" BOOL boss_fakewin_DeleteFileA(LPCSTR lpFileName)
    {BOSS_ASSERT("DeleteFileA준비중", false); return 0;}

    extern "C" BOOL boss_fakewin_MoveFileEx(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags)
    {
        BOSS_ASSERT("MoveFileEx준비중", false); return 0;
    }

    extern "C" BOOL boss_fakewin_MoveFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, DWORD dwFlags)
    {
        BOSS_ASSERT("dwFlags는 (MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)만 지원합니다",
            dwFlags == (MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED));

        Platform::File::Remove(lpNewFileName);
        return Platform::File::Rename(lpExistingFileName, lpNewFileName);
    }

    extern "C" DWORD boss_fakewin_GetTempPathA(DWORD nBufferLength, LPSTR lpBuffer)
    {
        BOSS_ASSERT("GetTempPathA준비중", false); return 0;
    }

    extern "C" UINT boss_fakewin_GetTempFileNameA(LPCSTR lpPathName, LPCSTR lpPrefixString, UINT uUnique, LPSTR lpTempFileName)
    {
        BOSS_ASSERT("GetTempFileNameA준비중", false); return 0;
    }

    extern "C" int boss_fakewin_MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
    {
        BOSS_ASSERT("CodePage는 CP_UTF8만 지원합니다", CodePage == CP_UTF8);
        BOSS_ASSERT("dwFlags는 MB_ERR_INVALID_CHARS만 지원합니다", dwFlags == MB_ERR_INVALID_CHARS);

        const WString& NewString = WString::FromChars(lpMultiByteStr, cbMultiByte);
        if(0 < cchWideChar)
        {
            BOSS_ASSERT("cchWideChar가 부족합니다", NewString.Length() + 1 <= cchWideChar);
            Memory::Copy(lpWideCharStr, (wchars) NewString, sizeof(wchar_t) * (NewString.Length() + 1));
        }

        return NewString.Length() + 1;
    }

    extern "C" int boss_fakewin_WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar,
        LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar)
    {
        BOSS_ASSERT("CodePage는 CP_UTF8만 지원합니다", CodePage == CP_UTF8);
        BOSS_ASSERT("dwFlags는 WC_ERR_INVALID_CHARS만 지원합니다", dwFlags == WC_ERR_INVALID_CHARS);
        BOSS_ASSERT("lpDefaultChar는 nullptr만 지원합니다", lpDefaultChar == nullptr);
        BOSS_ASSERT("lpUsedDefaultChar는 nullptr만 지원합니다", lpUsedDefaultChar == nullptr);

        const String& NewString = String::FromWChars(lpWideCharStr, cchWideChar);
        if(0 < cbMultiByte)
        {
            BOSS_ASSERT("cbMultiByte가 부족합니다", NewString.Length() + 1 <= cbMultiByte);
            Memory::Copy(lpMultiByteStr, (chars) NewString, sizeof(char) * (NewString.Length() + 1));
        }
        return NewString.Length() + 1;
    }

    extern "C" LONG boss_fakewin_RegCloseKey(HKEY hKey)
    {
        return 0;
    }

    extern "C" HANDLE boss_fakewin_RegisterEventSource(LPCTSTR,LPCTSTR) {BOSS_ASSERT("RegisterEventSource준비중", false); return 0;}

    extern "C" LONG boss_fakewin_RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
    {
        return 0;
    }

    extern "C" LONG boss_fakewin_RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
    {
        return 0;
    }

    extern "C" BOOL boss_fakewin_ReportEvent(HANDLE,WORD,WORD,DWORD,PSID,WORD,DWORD,LPCTSTR*,LPVOID) {BOSS_ASSERT("ReportEvent준비중", false); return 0;}

    extern "C" PVOID boss_fakewin_SecureZeroMemory(PVOID ptr, size_t cnt)
    {
        Memory::Set(ptr, 0x00, cnt);
        return ptr;
    }

    extern "C" LRESULT boss_fakewin_SendMessage(HWND,UINT,WPARAM,LPARAM) {BOSS_ASSERT("SendMessage준비중", false); return 0;}

    extern "C" BOOL boss_fakewin_SetFileAttributesW(LPCWSTR lpFileName, DWORD dwFileAttributes)
    {
        return true;
    }

    extern "C" void boss_fakewin_Sleep(DWORD dwMilliseconds)
    {
        Platform::Utility::Sleep(dwMilliseconds, false, false);
    }

    extern "C" void boss_fakewin_SleepEx(DWORD, BOOL)
    {
        BOSS_ASSERT("SleepEx준비중", false);
    }

    extern "C" BOOL boss_fakewin_VerifyVersionInfo(LPOSVERSIONINFOEX lpVersionInfo, DWORD dwTypeMask, DWORDLONG dwlConditionMask)
    {
        return true;
    }

    extern "C" BOOL boss_fakewin_SetEvent(HANDLE hEvent) {BOSS_ASSERT("SetEvent준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_ResetEvent(HANDLE hEvent) {BOSS_ASSERT("ResetEvent준비중", false); return 0;}
    extern "C" HANDLE boss_fakewin_CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName) {BOSS_ASSERT("CreateEvent준비중", false); return 0;}
    extern "C" HANDLE boss_fakewin_CreateEventW(LPSECURITY_ATTRIBUTES,BOOL,BOOL,LPCWSTR) {BOSS_ASSERT("CreateEventW준비중", false); return 0;}
    extern "C" DWORD boss_fakewin_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {BOSS_ASSERT("WaitForSingleObject준비중", false); return 0;}
    extern "C" DWORD boss_fakewin_WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds) {BOSS_ASSERT("WaitForMultipleObjects준비중", false); return 0;}

    extern "C" void boss_fakewin_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
    {
        //BOSS_ASSERT("DeleteCriticalSection준비중", false);
    }
    extern "C" BOOL boss_fakewin_TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
    {
        //BOSS_ASSERT("TryEnterCriticalSection준비중", false); return 0;
    }
    extern "C" void boss_fakewin_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
    {
        //BOSS_ASSERT("EnterCriticalSection준비중", false);
    }
    extern "C" void boss_fakewin_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
    {
        //BOSS_ASSERT("InitializeCriticalSection준비중", false);
        lpCriticalSection->DebugInfo = 0;
        lpCriticalSection->LockCount = 0;
        lpCriticalSection->RecursionCount = 0;
        lpCriticalSection->OwningThread = 0;
        lpCriticalSection->LockSemaphore = 0;
        lpCriticalSection->SpinCount = 0;
    }
    extern "C" void boss_fakewin_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
    {
        //BOSS_ASSERT("LeaveCriticalSection준비중", false);
    }

    static DWORD Common_WSAErrCode = 0;
    extern "C" int boss_fakewin_WSAGetLastError()
    {return Common_WSAErrCode;}
    extern "C" void boss_fakewin_WSASetLastError(int iError)
    {Common_WSAErrCode = iError;}

    extern "C" int boss_fakewin_WSACleanup()
    {
        return 0;
    }

    extern "C" int boss_fakewin_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
    {
        Memory::Set(lpWSAData, 0x00, sizeof(WSADATA));
        lpWSAData->wVersion = wVersionRequested;
        return 0;
    }

    extern "C" INT boss_fakewin_WSAStringToAddressA(LPSTR,INT,LPWSAPROTOCOL_INFO,LPSOCKADDR,LPINT) {BOSS_ASSERT("WSAStringToAddressA준비중", false); return 0;}

    extern "C" BOOL boss_fakewin_QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency) {BOSS_ASSERT("QueryPerformanceFrequency준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount) {BOSS_ASSERT("QueryPerformanceCounter준비중", false); return 0;}

    extern "C" void boss_fakewin_ZeroMemory(PVOID Destination, ULONG_PTR Length) {BOSS_ASSERT("ZeroMemory준비중", false);}
    extern "C" int boss_fakewin_GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject) {BOSS_ASSERT("GetObject준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_DeleteObject(HGDIOBJ hObject) {BOSS_ASSERT("DeleteObject준비중", false); return 0;}
    extern "C" HDC boss_fakewin_GetDC(HWND hWnd) {BOSS_ASSERT("GetDC준비중", false); return 0;}
    extern "C" int boss_fakewin_ReleaseDC(HWND hWnd, HDC hDC) {BOSS_ASSERT("ReleaseDC준비중", false); return 0;}
    extern "C" HBITMAP boss_fakewin_CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT iUsage, VOID** ppvBits, HANDLE hSection, DWORD dwOffset) {BOSS_ASSERT("CreateDIBSection준비중", false); return 0;}

    extern "C" DWORD boss_fakewin_TlsAlloc(void) {BOSS_ASSERT("TlsAlloc준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_TlsFree(DWORD dwTlsIndex) {BOSS_ASSERT("TlsFree준비중", false); return 0;}
    extern "C" LPVOID boss_fakewin_TlsGetValue(DWORD dwTlsIndex) {BOSS_ASSERT("TlsGetValue준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue) {BOSS_ASSERT("TlsSetValue준비중", false); return 0;}

    extern "C" HANDLE boss_fakewin_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) {BOSS_ASSERT("CreateMutexA준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_ReleaseMutex(HANDLE hMutex) {BOSS_ASSERT("ReleaseMutex준비중", false); return 0;}

    extern "C" HANDLE boss_fakewin_CreateSemaphoreA(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName)
    {BOSS_ASSERT("CreateSemaphoreA준비중", false); return 0;}
    extern "C" BOOL boss_fakewin_ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount)
    {BOSS_ASSERT("ReleaseSemaphore준비중", false); return 0;}
    extern "C" void boss_fakewin_GetStartupInfo(LPSTARTUPINFO lpStartupInfo)
    {BOSS_ASSERT("GetStartupInfo준비중", false);}
    extern "C" BOOL boss_fakewin_CreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
        BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
        LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
    {BOSS_ASSERT("CreateProcessA준비중", false); return 0;}

    extern "C" int boss_fakewin_waccess(const wchar_t* path, int mode)
    {
        BOSS_ASSERT("mode는 0만 지원합니다", mode == 0);

        return (Platform::File::CanAccess(path))? 0 : -1;
    }

    extern "C" int boss_fakewin_chmod(const char* filename, int pmode)
    {
	    BOSS_ASSERT("_chmod준비중", false);
        //if(S_IWRITE & pmode)
        //    return (Platform::File::CanWritable(filename))? 0 : -1;
        //if(S_IREAD & pmode)
        //    return (Platform::File::CanAccess(filename))? 0 : -1;
        return -1;
    }
	
    extern "C" int boss_fakewin_wchmod(const wchar_t* filename, int pmode)
    {
        if(S_IWRITE & pmode)
            return (Platform::File::CanWritable(filename))? 0 : -1;
        if(S_IREAD & pmode)
            return (Platform::File::CanAccess(filename))? 0 : -1;
        return -1;
    }

    extern "C" FILE* boss_fakewin_wfopen(const wchar_t* filename, const wchar_t* mode)
    {
        return boss_fakewin_fopen(String::FromWChars(filename), String::FromWChars(mode));
    }

    extern "C" errno_t boss_fakewin_wfopen_s(FILE** pfile, const wchar_t* filename, const wchar_t* mode)
    {
        if(pfile)
        {
            *pfile = boss_fakewin_wfopen(filename, mode);
            return (*pfile)? 0 : EINVAL;
        }
        return EINVAL;
    }

    extern "C" FILE* boss_fakewin_fopen(char const* filename, char const* mode)
    {
        return (FILE*) boss_fopen(filename, mode);
    }

    extern "C" errno_t boss_fakewin_fopen_s(FILE** pfile, char const* filename, char const* mode)
    {
        if(pfile)
        {
            *pfile = boss_fakewin_fopen(filename, mode);
            return (*pfile)? 0 : EINVAL;
        }
        return EINVAL;
    }

    extern "C" int boss_fakewin_fseek(FILE* stream, long int offset, int origin)
    {
        return boss_fseek(stream, offset, origin);
    }

    extern "C" long int boss_fakewin_ftell(FILE* stream)
    {
        return boss_ftell(stream);
    }

    extern "C" void boss_fakewin_rewind(FILE* stream)
    {
        boss_rewind(stream);
    }

    extern "C" size_t boss_fakewin_fread(void* ptr, size_t size, size_t count, FILE* stream)
    {
        return boss_fread(ptr, size, count, stream);
    }

    extern "C" size_t boss_fakewin_fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
    {
        return boss_fwrite(ptr, size, count, stream);
    }

    extern "C" int boss_fakewin_fgetc(FILE* stream)
    {
        return boss_fgetc(stream);
    }

    extern "C" char* boss_fakewin_fgets(char* str, int num, FILE* stream)
    {
        return boss_fgets(str, num, stream);
    }

    extern "C" int boss_fakewin_ungetc(int character, FILE* stream)
    {
        return boss_ungetc(character, stream);
    }

    extern "C" int boss_fakewin_fclose(FILE* stream)
    {
        return boss_fclose(stream);
    }

    extern "C" int boss_fakewin_wopen(const wchar_t* filename, int oflag, int pmode)
    {
        int TotalOFlags = O_RDONLY | O_WRONLY | O_RDWR | O_APPEND | O_CREAT |
            O_EXCL | O_TRUNC | O_UNKNOWN | O_BINARY | O_NOINHERIT | O_TEMPORARY;
        BOSS_ASSERT(String::Format("oflag(0x%08X)가 지원수준을 벗어납니다", oflag),
            !((oflag & 0x7FFFF) & ~TotalOFlags));
        const bool Writable = !!(pmode & S_IWRITE) | !!(oflag & O_CREAT);
        const bool Append = !!(oflag & O_APPEND);
        const bool Exclusive = (!!(oflag & O_WRONLY) | !!(oflag & O_RDWR)) & !!(oflag & O_EXCL);
        const bool Truncate = !!(oflag & O_TRUNC);

        return Platform::File::FDOpen(filename, Writable, Append, Exclusive, Truncate);
    }

    extern "C" int boss_fakewin_close(int fd)
    {
        return Platform::File::FDClose(fd)? 0 : -1;
    }

    extern "C" long boss_fakewin_read(int fd, void* buffer, unsigned int count)
    {
        return Platform::File::FDRead(fd, buffer, count);
    }

    extern "C" long boss_fakewin_write(int fd, const void* buffer, unsigned int count)
    {
        return Platform::File::FDWrite(fd, buffer, count);
    }

    extern "C" intptr_t boss_fakewin_get_osfhandle(int fd)
    {
        return fd;
    }

    extern "C" long boss_fakewin_lseek(int fd, long offset, int origin)
    {
        return Platform::File::FDSeek(fd, offset, origin);
    }

    extern "C" __int64 boss_fakewin_lseeki64(int fd, __int64 offset, int origin)
    {
        return Platform::File::FDSeek(fd, offset, origin);
    }

    extern "C" void* boss_fakewin_lfind(const void* key, const void* base, unsigned int* num, unsigned int width, int (*compare)(const void*, const void*))
    {
        BOSS_ASSERT("lfind준비중", false);
        return 0;
    }

    extern "C" errno_t boss_fakewin_chsize_s(int fd, __int64 size)
    {
        return Platform::File::FDResize(fd, size)? 0 : -1;
    }

    extern "C" int boss_fakewin_stat(const char* file_name, struct boss_fakewin_struct_stat* _Stat)
    {
        BOSS_ASSERT("_stat준비중", false);
        return 0;
    }

    extern "C" int boss_fakewin_stat64(const char* file_name, struct boss_fakewin_struct_stat64* _Stat)
    {
        BOSS_ASSERT("_stat64준비중", false);
        return 0;
    }

    extern "C" int boss_fakewin_fstat(int fd, struct boss_fakewin_struct_stat* _Stat)
    {
        uint64 GetSize, GetCreateTime, GetAccessTime, GetModifyTime;
        const DWORD FileAttributes = Platform::File::FDAttributes(fd,
            &GetSize, &GetCreateTime, &GetAccessTime, &GetModifyTime);
        if(FileAttributes == -1)
            return -1;

        Memory::Set(_Stat, 0x00, sizeof(struct boss_fakewin_struct_stat));
        _Stat->st_mode = 0;
        if(FileAttributes & FILE_ATTRIBUTE_DIRECTORY) _Stat->st_mode |= S_IFDIR;
        else
        {
            _Stat->st_mode |= S_IFREG;
            if(FileAttributes & FILE_ATTRIBUTE_READONLY) _Stat->st_mode |= S_IREAD;
            else _Stat->st_mode |= S_IREAD | S_IWRITE;
        }

        _Stat->st_dev = 0;
        _Stat->st_ino = 0;
        _Stat->st_nlink = 1; // No NTFS
        _Stat->st_uid = 0;
        _Stat->st_gid = 0;
        _Stat->st_rdev = 0;
        _Stat->st_size = GetSize;
        #if BOSS_MAC_OSX
            _Stat->st_atimespec.tv_sec = WindowToEpoch(GetAccessTime / 10000) / 1000;
            _Stat->st_mtimespec.tv_sec = WindowToEpoch(GetModifyTime / 10000) / 1000;
            _Stat->st_ctimespec.tv_sec = WindowToEpoch(GetCreateTime / 10000) / 1000;
        #else
            _Stat->st_atime = WindowToEpoch(GetAccessTime / 10000) / 1000;
            _Stat->st_mtime = WindowToEpoch(GetModifyTime / 10000) / 1000;
            _Stat->st_ctime = WindowToEpoch(GetCreateTime / 10000) / 1000;
        #endif
        return 0;
    }

    extern "C" int boss_fakewin_fstat64(int fd, struct boss_fakewin_struct_stat64* _Stat)
    {
        uint64 GetSize, GetCreateTime, GetAccessTime, GetModifyTime;
        const DWORD FileAttributes = Platform::File::FDAttributes(fd,
            &GetSize, &GetCreateTime, &GetAccessTime, &GetModifyTime);
        if(FileAttributes == -1)
            return -1;

        Memory::Set(_Stat, 0x00, sizeof(struct boss_fakewin_struct_stat64));
        _Stat->st_mode = 0;
        if(FileAttributes & FILE_ATTRIBUTE_DIRECTORY) _Stat->st_mode |= S_IFDIR;
        else
        {
            _Stat->st_mode |= S_IFREG;
            if(FileAttributes & FILE_ATTRIBUTE_READONLY) _Stat->st_mode |= S_IREAD;
            else _Stat->st_mode |= S_IREAD | S_IWRITE;
        }

        _Stat->st_dev = 0;
        _Stat->st_ino = 0;
        _Stat->st_nlink = 1; // No NTFS
        _Stat->st_uid = 0;
        _Stat->st_gid = 0;
        _Stat->st_rdev = 0;
        _Stat->st_size = GetSize;
        #if BOSS_MAC_OSX
            _Stat->st_atimespec.tv_sec = WindowToEpoch(GetAccessTime / 10000) / 1000;
            _Stat->st_mtimespec.tv_sec = WindowToEpoch(GetModifyTime / 10000) / 1000;
            _Stat->st_ctimespec.tv_sec = WindowToEpoch(GetCreateTime / 10000) / 1000;
        #else
            _Stat->st_atime = WindowToEpoch(GetAccessTime / 10000) / 1000;
            _Stat->st_mtime = WindowToEpoch(GetModifyTime / 10000) / 1000;
            _Stat->st_ctime = WindowToEpoch(GetCreateTime / 10000) / 1000;
        #endif
        return 0;
    }

    extern "C" int boss_fakewin_wchdir(const wchar_t* dirname)
    {
        #if BOSS_WINDOWS
            return _wchdir(dirname);
        #else
            BOSS_ASSERT("준비중", false); return 0;
        #endif
    }

    extern "C" int boss_fakewin_wmkdir(const wchar_t* dirname)
    {
        return Platform::File::CreateDir(dirname)? 0 : -1;
    }

    extern "C" int boss_fakewin_wrmdir(const wchar_t* dirname)
    {
        if(!Platform::File::ExistForDir(String::FromWChars(dirname)))
        {
            boss_fakewin_SetLastError(ERROR_DIRECTORY);
            return -1;
        }

        return Platform::File::RemoveDir(dirname)? 0 : -1;
    }

    extern "C" int boss_fakewin_wunlink(const wchar_t* filename)
    {
        return Platform::File::Remove(filename)? 0 : -1;
    }

    extern "C" errno_t boss_fakewin_mktemp_s(char* _TemplateName, size_t _Size)
    {
        return Platform::File::Tempname(_TemplateName, _Size)? 0 : -1;
    }

    extern "C" wchar_t* boss_fakewin_wgetenv(const wchar_t* varname)
    {
        return (wchar_t*) L"";
    }

    extern "C" wchar_t* boss_fakewin_wgetcwd(wchar_t* buffer, int maxlen)
    {
        #if BOSS_WINDOWS
            return _wgetcwd(buffer, maxlen);
        #else
            BOSS_ASSERT("준비중", false); return 0;
        #endif
    }

    extern "C" void boss_fakewin_FD_SET(int fd, boss_fakewin_struct_fd_set* fdset)
    {
        fdset->fd_array[fdset->fd_count] = fd;
    }

    extern "C" int boss_fakewin_FD_ISSET(int fd, boss_fakewin_struct_fd_set* fdset)
	{
	    return (fdset->fd_array[fdset->fd_count] == fd);
	}

    extern "C" void boss_fakewin_FD_ZERO(boss_fakewin_struct_fd_set* fdset)
    {
        fdset->fd_count = 0;
    }

    extern "C" void* boss_fakewin_alloca(size_t _Size)
    {
        #if BOSS_WINDOWS
            return _alloca(_Size);
        #else
            static uint08s _;
            return _.AtDumping(0, _Size);
        #endif
    }

    extern "C" int boss_fakewin_fileno(FILE* _File)
    {
        #if BOSS_WINDOWS
            return _fileno(_File);
        #else
            BOSS_ASSERT("준비중", false); return 0;
        #endif
    }

    extern "C" int boss_fakewin_getch()
    {
        #if BOSS_WINDOWS
            return _getch();
        #else
            BOSS_ASSERT("준비중", false); return 0;
        #endif
    }

    extern "C" int boss_fakewin_getdrive()
    {
        return Platform::File::GetDriveCode();
    }

    extern "C" unsigned long boss_fakewin_lrotl(unsigned long _Val, int _Shift)
    {
        _Shift &= 0x1F;
        const unsigned long Result = (_Val << _Shift) | (_Val >> (32 - _Shift));

        #if BOSS_WINDOWS
            BOSS_ASSERT("_lrotl의 연산값에 문제가 있습니다", Result == _lrotl(_Val, _Shift));
        #endif

        return Result;
    }

    extern "C" unsigned long boss_fakewin_lrotr(unsigned long _Val, int _Shift)
    {
        _Shift &= 0x1F;
        const unsigned long Result = (_Val >> _Shift) | (_Val << (32 - _Shift));

        #if BOSS_WINDOWS
            BOSS_ASSERT("_lrotr의 연산값에 문제가 있습니다", Result == _lrotr(_Val, _Shift));
        #endif

        return Result;
    }	

    extern "C" unsigned int boss_fakewin_rotl(unsigned int _Val, int _Shift)
    {
        _Shift &= 0x1F;
        const unsigned int Result = (_Val >> _Shift) | (_Val << (32 - _Shift));

        #if BOSS_WINDOWS
            BOSS_ASSERT("_rotl의 연산값에 문제가 있습니다", Result == _rotl(_Val, _Shift));
        #endif

        return Result;
    }

    extern "C" int boss_fakewin_setmode(int _FileHandle, int _Mode)
    {
        #if BOSS_WINDOWS
            return _setmode(_FileHandle, _Mode);
        #else
            BOSS_ASSERT("준비중", false); return 0;
        #endif
    }

    extern "C" int boss_fakewin_stricmp(const char* str1, const char* str2)
    {
        return boss_stricmp(str1, str2);
    }

    extern "C" int boss_fakewin_strnicmp(const char* str1, const char* str2, size_t maxcount)
    {
        return boss_strnicmp(str1, str2, maxcount);
    }

    extern "C" int boss_fakewin_mbsicmp(const unsigned char* str1, const unsigned char* str2)
    {
        return boss_stricmp((const char*) str1, (const char*) str2);
    }

    extern "C" int boss_fakewin_wcsicmp(wchar_t const* str1, wchar_t const* str2)
    {
        return boss_wcsicmp(str1, str2);
    }

    extern "C" int boss_fakewin_vscprintf(const char* _Format, va_list _ArgList)
    {
        return boss_vsnprintf(nullptr, 0, _Format, _ArgList);
    }

    extern "C" int boss_fakewin_vsnprintf_s(char* _DstBuf, size_t _SizeInBytes, size_t _MaxCount, const char* _Format, va_list _ArgList)
    {
        BOSS_ASSERT("_MaxCount는 ((size_t) -1)만 지원합니다", _MaxCount == ((size_t) -1));

        return boss_vsnprintf(_DstBuf, _SizeInBytes, _Format, _ArgList);
    }

    extern "C" int boss_fakewin_vsnprintf(char* _DstBuf, size_t _SizeInBytes, const char* _Format, va_list _ArgList)
    {
        return boss_vsnprintf(_DstBuf, _SizeInBytes, _Format, _ArgList);
    }

    extern "C" int boss_fakewin_vsnwprintf(wchar_t* _DstBuf, size_t _SizeInBytes, const wchar_t* _Format, va_list _ArgList)
    {
        return boss_vsnwprintf(_DstBuf, _SizeInBytes, _Format, _ArgList);
    }

    extern "C" uintptr_t boss_fakewin_beginthreadex(void* security, unsigned stack_size, unsigned (*start_address)(void*), void* arglist, unsigned initflag, unsigned* thrdaddr)
    {
        BOSS_ASSERT("_beginthreadex준비중", false);
        return 0;
    }
    extern "C" char* boss_fakewin_fullpath(char* absPath, const char* relPath, size_t maxLength)
    {
        BOSS_ASSERT("_fullpath준비중", false);
        return 0;
    }
    extern "C" char* boss_fakewin_ltoa(long value, char* str, int radix)
    {
        BOSS_ASSERT("_ltoa준비중", false);
        return 0;
    }
    extern "C" char* boss_fakewin_ultoa(unsigned long value, char* str, int radix)
    {
        BOSS_ASSERT("_ultoa준비중", false);
        return 0;
    }
    extern "C" int boss_fakewin_isnan(double x)
    {
        #if BOSS_WINDOWS
            return _isnan(x);
        #else
            BOSS_ASSERT("_isnan준비중", false);
            return 0;
        #endif
    }
    extern "C" int boss_fakewin_finite(double x)
    {
        #if BOSS_WINDOWS
            return _finite(x);
        #else
            BOSS_ASSERT("_finite준비중", false);
            return 0;
        #endif
    }
    extern "C" void boss_fakewin_splitpath(const char* path, char* drive, char* dir, char* fname, char* ext)
    {
        BOSS_ASSERT("_splitpath준비중", false);
    }
    extern "C" __int64 boss_fakewin_strtoi64(const char* nptr, char** endptr, int base)
    {
        BOSS_ASSERT("_strtoi64준비중", false);
        return 0;
    }
    extern "C" unsigned __int64 boss_fakewin_strtoui64(const char* nptr, char** endptr, int base)
    {
        BOSS_ASSERT("_strtoui64준비중", false);
        return 0;
    }
    extern "C" errno_t boss_fakewin_set_errno(int value)
    {
        BOSS_ASSERT("_set_errno준비중", false);
        return 0;
    }
    extern "C" LPCH boss_fakewin_GetEnvironmentStrings(void)
    {
        return (LPCH)"";
    }
    extern "C" LPWCH boss_fakewin_GetEnvironmentStringsW(void)
    {
        return (LPWCH) L"";
    }
    extern "C" UINT boss_fakewin_GetSystemDirectoryA(LPSTR lpBuffer, UINT uSize)
    {
        BOSS_ASSERT("GetSystemDirectoryA준비중", false);
        return 0;
    }
    extern "C" UINT boss_fakewin_GetSystemDirectoryW(LPWSTR lpBuffer, UINT uSize)
    {
        BOSS_ASSERT("GetSystemDirectoryW준비중", false);
        return 0;
    }
    extern "C" UINT boss_fakewin_GetEnvironmentVariableA(LPCSTR lpName, LPSTR lpBuffer, UINT uSize)
    {
        BOSS_ASSERT("GetEnvironmentVariableA준비중", false);
        return 0;
    }
    extern "C" UINT boss_fakewin_GetEnvironmentVariableW(LPCWSTR lpName, LPWSTR lpBuffer, UINT uSize)
    {
        BOSS_ASSERT("GetEnvironmentVariableW준비중", false);
        return 0;
    }
	extern "C" BOOL boss_fakewin_ReadConsoleA(HANDLE hConsoleInput, LPVOID lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, PCONSOLE_READCONSOLE_CONTROL pInputControl)
	{
	    BOSS_ASSERT("ReadConsoleA준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_ReadConsoleW(HANDLE hConsoleInput, LPVOID lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, PCONSOLE_READCONSOLE_CONTROL pInputControl)
	{
	    BOSS_ASSERT("ReadConsoleW준비중", false);
	    return 0;
	}
	extern "C" VOID boss_fakewin_GetSystemTime(LPSYSTEMTIME lpSystemTime)
	{
	    BOSS_ASSERT("GetSystemTime준비중", false);
	}
	extern "C" VOID boss_fakewin_SwitchToFiber(LPVOID lpFiber)
	{
	    BOSS_ASSERT("SwitchToFiber준비중", false);
	}
	extern "C" VOID boss_fakewin_DeleteFiber(LPVOID lpFiber)
	{
	    BOSS_ASSERT("DeleteFiber준비중", false);
	}
	extern "C" BOOL boss_fakewin_ConvertFiberToThread(void)
	{
	    BOSS_ASSERT("ConvertFiberToThread준비중", false);
	    return 0;
	}
	extern "C" LPVOID boss_fakewin_ConvertThreadToFiber(LPVOID lpParameter)
	{
	    BOSS_ASSERT("ConvertThreadToFiber준비중", false);
	    return 0;
	}
	extern "C" LPVOID boss_fakewin_CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize,DWORD dwFlags, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter)
	{
	    BOSS_ASSERT("CreateFiberEx준비중", false);
	    return 0;
	}
	extern "C" LPVOID boss_fakewin_ConvertThreadToFiberEx(LPVOID lpParameter, DWORD dwFlags)
	{
	    BOSS_ASSERT("ConvertThreadToFiberEx준비중", false);
	    return 0;
	}
	extern "C" LPVOID boss_fakewin_CreateFiber(SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter)
	{
	    BOSS_ASSERT("CreateFiber준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode)
	{
	    BOSS_ASSERT("GetConsoleMode준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_SetConsoleMode(HANDLE hConsoleHandle, LPDWORD dwMode)
	{
	    BOSS_ASSERT("SetConsoleMode준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
	    BOSS_ASSERT("ReadFile준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_ReadFileEx(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
	    BOSS_ASSERT("ReadFileEx준비중", false);
	    return 0;
	}
	extern "C" DWORD boss_fakewin_GetFileType(HANDLE hFile)
	{
	    BOSS_ASSERT("GetFileType준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_PeekNamedPipe(HANDLE hNamedPipe, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesRead, LPDWORD lpTotalBytesAvail, LPDWORD lpBytesLeftThisMessage)
	{
	    BOSS_ASSERT("PeekNamedPipe준비중", false);
	    return 0;
	}
	extern "C" DWORD boss_fakewin_timeGetTime(void)
	{
	    BOSS_ASSERT("timeGetTime준비중", false);
	    return 0;
	}
	extern "C" PVOID boss_fakewin_GetFiberData(void)
	{
	    //return *(PVOID *) (ULONG_PTR) __readfsdword (0x10);
	    BOSS_ASSERT("GetFiberData준비중", false);
	    return 0;
	}
	extern "C" PVOID boss_fakewin_GetCurrentFiber(void)
	{
	    //return (PVOID) (ULONG_PTR) __readfsdword (0x10);
	    BOSS_ASSERT("GetCurrentFiber준비중", false);
	    return 0;
	}
	extern "C" unsigned short boss_fakewin_byteswap_ushort(unsigned short _Number)
	{
	    BOSS_ASSERT("_byteswap_ushort준비중", false);
	    return 0;
	}
	extern "C" unsigned long boss_fakewin_byteswap_ulong(unsigned long _Number)
	{
	    BOSS_ASSERT("_byteswap_ulong준비중", false);
	    return 0;
	}
	extern "C" unsigned __int64 boss_fakewin_byteswap_uint64(unsigned __int64 _Number)
	{
	    BOSS_ASSERT("_byteswap_uint64준비중", false);
	    return 0;
	}
	extern "C" size_t boss_fakewin_strlen(const char *str)
	{
	    return boss_strlen(str);
	}
	extern "C" size_t boss_fakewin_wcslen(const wchar_t *str)
	{
        return boss_wcslen(str);
	}
	extern "C" char* boss_fakewin_strerror(int errnum)
	{
	    BOSS_ASSERT("strerror준비중", false);
        return "";
	}
	extern "C" errno_t boss_fakewin_strerror_s(char *buf, rsize_t bufsz, errno_t errnum)
	{
	    BOSS_ASSERT("strerror_s준비중", false);
	    return 0;	
	}
	extern "C" errno_t boss_fakewin_strcpy_s(char *strDestination, size_t numberOfElements, const char *strSource)
	{
		BOSS_ASSERT("strcpy_s준비중", false);
	    //for (size_t i = 0; i < numberOfElements; ++i)
		//	dest[i] = sour[i];
	    return 0;	
	}
	extern "C" errno_t boss_fakewin_wcscpy_s(wchar_t *strDestination, size_t numberOfElements, const wchar_t *strSource)
	{
		BOSS_ASSERT("wcscpy_s준비중", false);	
	    //for (size_t i = 0; i < numberOfElements; ++i)
		//	dest[i] = sour[i];
	    return 0;	
	}	
	extern "C" char* boss_fakewin_strcpy(char *strDestination, const char *strSource)
	{
        char* temp = strDestination;
        while (*(temp++) = *(strSource++));
        return strDestination;
	}
	extern "C" wchar_t* boss_fakewin_wcscpy(wchar_t *strDestination, const wchar_t *strSource)
	{
        wchar_t* temp = strDestination;
        while (*(temp++) = *(strSource++));
        return strDestination;
	}
	extern "C" char* boss_fakewin_strdup(const char *strSource)
	{
        BOSS_ASSERT("_strdup준비중", false);
        return 0;
	}
	extern "C" wchar_t* boss_fakewin_wcsdup(const wchar_t *strSource)
	{
        BOSS_ASSERT("_wcsdup준비중", false);
        return 0;
	}
	extern "C" char* boss_fakewin_strpbrk(const char *str, const char *strCharSet)
	{
        BOSS_ASSERT("strpbrk준비중", false);
        return 0;
	}
	extern "C" wchar_t* boss_fakewin_wcspbrk(const wchar_t *str, const wchar_t *strCharSet)
	{
        BOSS_ASSERT("wcspbrk준비중", false);
        return 0;
	}
	extern "C" VOID boss_fakewin_ber_free(BerElement *pBerElement, INT fbuf)
	{
        BOSS_ASSERT("ber_free준비중", false);
	}			
	extern "C" BOOL boss_fakewin_GetVersionExA(LPOSVERSIONINFOA)
	{
	    BOSS_ASSERT("GetVersionExA준비중", false);
	    return 0;
	}
	extern "C" BOOL boss_fakewin_GetVersionExW(LPOSVERSIONINFOW)
	{
	    BOSS_ASSERT("GetVersionExW준비중", false);
	    return 0;
	}
	extern "C" int boss_fakewin_WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
	    BOSS_ASSERT("WSAIoctl준비중", false);
	    return 0;
	}
	extern "C" PWCHAR boss_fakewin_ldap_err2stringW(ULONG err)
	{
	    BOSS_ASSERT("ldap_err2stringW준비중", false);
        return 0;
	}
	extern "C" PCHAR boss_fakewin_ldap_err2stringA(ULONG err)
	{
	    BOSS_ASSERT("ldap_err2stringA준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_get_option( LDAP *ld, int option, void *outvalue )
    {
	    BOSS_ASSERT("ldap_get_option준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_get_optionW( LDAP *ld, int option, void *outvalue )
    {
	    BOSS_ASSERT("ldap_get_optionW준비중", false);
        return 0;
	}
    extern "C" ULONG boss_fakewin_ldap_set_option( LDAP *ld, int option, const void *outvalue )
    {
	    BOSS_ASSERT("ldap_set_option준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_set_optionW( LDAP *ld, int option, const void *outvalue )
    {
	    BOSS_ASSERT("ldap_set_optionW준비중", false);
        return 0;
	}
    extern "C" LDAP* boss_fakewin_ldap_initW(const PWSTR HostName, ULONG PortNumber)
    {
	    BOSS_ASSERT("ldap_initW준비중", false);
        return 0;
	}
	extern "C" LDAP* boss_fakewin_ldap_init(const PSTR HostName, ULONG PortNumber)
    {
	    BOSS_ASSERT("ldap_init준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_simple_bindW( LDAP *ld, PWSTR dn, PWSTR passwd )
	{
		BOSS_ASSERT("ldap_simple_bindW준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_simple_bindA( LDAP *ld, PSTR dn, PSTR passwd )
	{
		BOSS_ASSERT("ldap_simple_bindA준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_simple_bind_sW( LDAP *ld, PWSTR dn, PWSTR passwd )
	{
		BOSS_ASSERT("ldap_simple_bind_sW준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_simple_bind_sA( LDAP *ld, PSTR dn, PSTR passwd )
	{
		BOSS_ASSERT("ldap_simple_bind_sA준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_searchW(LDAP *ld, const PWSTR base, ULONG scope, const PWSTR filter, PZPWSTR attrs, LONG attrsonly)
	{
		BOSS_ASSERT("ldap_searchW준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_searchA(LDAP *ld, const PSTR base, ULONG scope, const PSTR filter, PZPSTR attrs, LONG attrsonly)
	{
		BOSS_ASSERT("ldap_searchA준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_search_sW(LDAP *ld, const PWSTR base, ULONG scope, const PWSTR filter, PZPWSTR attrs, LONG attrsonly, LDAPMessage** res)
	{
		BOSS_ASSERT("ldap_search_sW준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_search_sA(LDAP *ld, const PSTR base, ULONG scope, const PSTR filter, PZPSTR attrs, LONG attrsonly, LDAPMessage** res)
	{
		BOSS_ASSERT("ldap_search_sA준비중", false);
        return 0;
	}
	extern "C" LDAPMessage* boss_fakewin_ldap_first_entry(LDAP *ld, LDAPMessage *res)
	{
		BOSS_ASSERT("ldap_first_entry준비중", false);
        return 0;
	}
	extern "C" PWCHAR boss_fakewin_ldap_get_dnW(LDAP *ld, LDAPMessage *entry)
	{
		BOSS_ASSERT("ldap_first_entry준비중", false);
        return 0;
	}
	extern "C" PCHAR boss_fakewin_ldap_get_dn(LDAP *ld, LDAPMessage *entry)
	{
		BOSS_ASSERT("ldap_first_entry준비중", false);
        return 0;
	}	
	extern "C" VOID boss_fakewin_ldap_memfreeW(PWCHAR Block)
	{
		BOSS_ASSERT("ldap_first_entry준비중", false);
	}
	extern "C" VOID boss_fakewin_ldap_memfree(PCHAR Block)
	{
		BOSS_ASSERT("ldap_first_entry준비중", false);
	}
	extern "C" PWCHAR boss_fakewin_ldap_first_attributeW(LDAP *ld, LDAPMessage* entry, BerElement** ptr)
	{
		BOSS_ASSERT("ldap_first_attributeW준비중", false);
        return 0;
	}
	extern "C" PCHAR boss_fakewin_ldap_first_attribute(LDAP *ld, LDAPMessage* entry, BerElement** ptr)
	{
		BOSS_ASSERT("ldap_first_attribute준비중", false);
        return 0;
	}
	extern "C" PWCHAR boss_fakewin_ldap_next_attributeW(LDAP *ld, LDAPMessage* entry, BerElement* ptr)
	{
		BOSS_ASSERT("ldap_next_attributeW준비중", false);
        return 0;
	}
	extern "C" PCHAR boss_fakewin_ldap_next_attribute(LDAP *ld, LDAPMessage* entry, BerElement* ptr)
	{
		BOSS_ASSERT("ldap_next_attribute준비중", false);
        return 0;
	}	
	extern "C" struct berval** boss_fakewin_ldap_get_values_lenW(LDAP *ExternalHandle, LDAPMessage* Message, const PWSTR attr)
	{
		BOSS_ASSERT("ldap_get_values_lenW준비중", false);
        return 0;
	}
	extern "C" struct berval** boss_fakewin_ldap_get_values_len(LDAP *ExternalHandle, LDAPMessage* Message, const PSTR attr)
	{
		BOSS_ASSERT("ldap_get_values_len준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_value_free_len(struct berval **vals)
	{
		BOSS_ASSERT("ldap_next_attribute준비중", false);
        return 0;
	}
	extern "C" LDAPMessage* boss_fakewin_ldap_next_entry(LDAP *ld, LDAPMessage *entry)
	{
		BOSS_ASSERT("ldap_next_entry준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_msgfree(LDAPMessage *res)
	{
		BOSS_ASSERT("ldap_msgfree준비중", false);
        return 0;
	}
    extern "C" ULONG boss_fakewin_ldap_unbind(LDAP *ld)
	{
		BOSS_ASSERT("ldap_unbind준비중", false);
        return 0;
	}
	extern "C" ULONG boss_fakewin_ldap_unbind_s(LDAP *ld)
	{
		BOSS_ASSERT("ldap_unbind_s준비중", false);
        return 0;
	}


    #if BOSS_MAC_OSX || BOSS_IPHONE
        namespace std
        {
            #define stringstream
            /*boss_fakewin_stringstream::boss_fakewin_stringstream()
            {
                mStr = (void*) new std::string();
            }
            boss_fakewin_stringstream::~boss_fakewin_stringstream()
            {
                delete (std::string*) mStr;
            }
            void boss_fakewin_stringstream::str(const char* s)
            {
                *((std::string*) mStr) = std::string(s);
            }*/
        }
    #endif

    #if !BOSS_LINUX & !BOSS_ANDROID
        namespace std
        {
            #undef ifstream
            /*boss_fakewin_ifstream::boss_fakewin_ifstream(const char* filename, ios_base::openmode mode)
            {
                std::string modeStr("r");
                printf("Open file (read): %s\n", filename);
                if (mode & ios_base::binary)
                    modeStr += "b";

                f = boss_fakewin_fopen(filename, modeStr.c_str());
                if (f == NULL)
                {
                    printf("Can't open file: %s\n", filename);
                    return;
                }

                boss_fakewin_fseek(f, 0, SEEK_END);
                size_t sz = boss_fakewin_ftell(f);
                if (sz > 0)
                {
                    char* buf = (char*) malloc(sz);
                    boss_fakewin_fseek(f, 0, SEEK_SET);
                    if (boss_fakewin_fread(buf, 1, sz, f) == sz)
                    {
                        this->str(std::string(buf, sz).c_str());
                    }
                    free(buf);
                }
            }

            boss_fakewin_ifstream::~boss_fakewin_ifstream()
            {
                close();
            }

            bool boss_fakewin_ifstream::is_open() const
            {
                return f != NULL;
            }

            void boss_fakewin_ifstream::close()
            {
                if (f)
                    boss_fakewin_fclose(f);
                f = NULL;
                this->str("");
            }*/
        }
    #endif
#endif
