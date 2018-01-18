#include <boss.hpp>

#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

#if BOSS_WINDOWS
    #include <windows.h>
#elif BOSS_LINUX
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <strings.h>
#elif BOSS_MAC_OSX || BOSS_IPHONE
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <CoreFoundation/CoreFoundation.h>
#elif BOSS_ANDROID
    #include <dirent.h>
    #include <android/asset_manager_jni.h>
#endif

extern "C"
{
    #if BOSS_WINDOWS || BOSS_LINUX || BOSS_ANDROID
        extern int isalpha(int);
        extern int isdigit(int);
        extern int isalnum(int);
    #endif

    #if BOSS_ANDROID
        extern double strtod(const char*, char**);
    #else
        extern double atof(const char*);
    #endif

    extern size_t strlen(const char*);
    extern size_t wcslen(const wchar_t*);

    #if BOSS_MAC_OSX || BOSS_IPHONE
        extern int vasprintf(char**, const char*, va_list);
    #else
        extern int vsnprintf(char*, size_t, const char*, va_list);
    #endif

    #if BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        extern int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
    #else
        extern int _vsnwprintf(wchar_t*, size_t, const wchar_t*, va_list);
    #endif

    extern int strcmp(const char*, const char*);
    extern int strncmp(const char*, const char*, size_t);
    #if BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        extern int strcasecmp(const char *, const char *);
        extern int strncasecmp(const char *, const char *, size_t);
    #else
        extern int stricmp(const char*, const char*);
        extern int strnicmp(const char*, const char*, size_t);
    #endif

    extern int wcscmp(const wchar_t*, const wchar_t*);
    extern int wcsncmp(const wchar_t*, const wchar_t*, size_t);
    #if BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        extern int wcscasecmp(const wchar_t*, const wchar_t*);
        extern int wcsncasecmp(const wchar_t*, const wchar_t*, size_t);
    #else
        extern int _wcsicmp(const wchar_t*, const wchar_t*);
        extern int _wcsnicmp(const wchar_t*, const wchar_t*, size_t);
    #endif

    // MFC프로젝트시의 문제해결
    extern FILE* fopen(char const*, char const*);
    void _except_handler4_common() {}
}

////////////////////////////////////////////////////////////////////////////////
// String
////////////////////////////////////////////////////////////////////////////////

extern "C" int boss_isalpha(int c)
{
    return isalpha(c);
}

extern "C" int boss_isdigit(int c)
{
    return isdigit(c);
}

extern "C" int boss_isalnum(int c)
{
    return isalnum(c);
}

extern "C" double boss_atof(const char* str)
{
    #if BOSS_ANDROID
        return strtod(str, nullptr);
    #else
        return atof(str);
    #endif
}

extern "C" size_t boss_strlen(const char* str)
{
    return strlen(str);
}

extern "C" size_t boss_wcslen(const wchar_t* str)
{
    return wcslen(str);
}

extern "C" int boss_snprintf(char* str, size_t n, const char* format, ...)
{
    va_list Args;
    va_start(Args, format);
    const sint32 Size = boss_vsnprintf(str, n, format, Args);
    va_end(Args);
    return Size;
}

extern "C" int boss_vsnprintf(char* str, size_t n, const char* format, boss_va_list args)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE
        char* NewString = nullptr;
        const sint32 Size = vasprintf(&NewString, format, args);

        sint32 CopiedSize = Size;
        if(str)
        {
            if(CopiedSize > n - 1) CopiedSize = n - 1;
            Memory::Copy(str, NewString, CopiedSize);
            str[CopiedSize] = '\0';
        }

        Memory::Free(NewString);
        return CopiedSize;
    #else
        return vsnprintf(str, n, format, args);
    #endif
}

extern "C" int boss_snwprintf(wchar_t* str, size_t n, const wchar_t* format, ...)
{
    va_list Args;
    va_start(Args, format);
    const sint32 Size = boss_vsnwprintf(str, n, format, Args);
    va_end(Args);
    return Size;
}

extern "C" int boss_vsnwprintf(wchar_t* str, size_t n, const wchar_t* format, boss_va_list args)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        return vswprintf(str, n, format, args);
    #else
        return _vsnwprintf(str, n, format, args);
    #endif
}

extern "C" int boss_strcmp(const char* str1, const char* str2)
{
    return strcmp(str1, str2);
}

extern "C" int boss_strncmp(const char* str1, const char* str2, size_t maxcount)
{
    return strncmp(str1, str2, maxcount);
}

extern "C" int boss_stricmp(const char* str1, const char* str2)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        return strcasecmp(str1, str2);
    #else
        return stricmp(str1, str2);
    #endif
}

extern "C" int boss_strnicmp(const char* str1, const char* str2, size_t maxcount)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        return strncasecmp(str1, str2, maxcount);
    #else
        return strnicmp(str1, str2, maxcount);
    #endif
}

extern "C" int boss_wcscmp(const wchar_t* str1, const wchar_t* str2)
{
    return wcscmp(str1, str2);
}

extern "C" int boss_wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t maxcount)
{
    return wcsncmp(str1, str2, maxcount);
}

extern "C" int boss_wcsicmp(const wchar_t* str1, const wchar_t* str2)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        return wcscasecmp(str1, str2);
    #else
        return _wcsicmp(str1, str2);
    #endif
}

extern "C" int boss_wcsnicmp(const wchar_t* str1, const wchar_t* str2, size_t maxcount)
{
    #if BOSS_LINUX || BOSS_MAC_OSX || BOSS_IPHONE || BOSS_ANDROID
        return wcsncasecmp(str1, str2, maxcount);
    #else
        return _wcsnicmp(str1, str2, maxcount);
    #endif
}

////////////////////////////////////////////////////////////////////////////////
// File
////////////////////////////////////////////////////////////////////////////////

class FileClass
{
public:
    FileClass()
    {
        mFileID = -1;
        mTypeAssets = false;
        mFilePointer = nullptr;
        mNeedSave = false;
        mContent = nullptr;
        mFileSize = -1;
        mFileOffset = 0;
    }

    ~FileClass()
    {
        if(mTypeAssets)
        {
            #if BOSS_WINDOWS
                fclose((FILE*) mFilePointer);
            #elif BOSS_LINUX
                fclose((FILE*) mFilePointer);
            #elif BOSS_ANDROID
                AAsset_close((AAsset*) mFilePointer);
            #elif BOSS_MAC_OSX || BOSS_IPHONE
                fclose((FILE*) mFilePointer);
            #else
                #error 준비되지 않은 플랫폼입니다
            #endif
        }
        else
        {
            if(mNeedSave && mContent)
                fwrite(mContent->AtDumping(0, mFileSize), 1, mFileSize, (FILE*) mFilePointer);
            fclose((FILE*) mFilePointer);
        }
        delete mContent;
    }

public:
    void ValidSize()
    {
        if(mFileSize == -1)
        {
            if(mTypeAssets)
            {
                #if BOSS_WINDOWS
                    fseek((FILE*) mFilePointer, 0, SEEK_END);
                    mFileSize = ftell((FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #elif BOSS_LINUX
                    fseek((FILE*) mFilePointer, 0, SEEK_END);
                    mFileSize = ftell((FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #elif BOSS_ANDROID
                    mFileSize = AAsset_getLength((AAsset*) mFilePointer);
                #elif BOSS_MAC_OSX || BOSS_IPHONE
                    fseek((FILE*) mFilePointer, 0, SEEK_END);
                    mFileSize = ftell((FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #else
                    #error 준비되지 않은 플랫폼입니다
                #endif
            }
            else
            {
                fseek((FILE*) mFilePointer, 0, SEEK_END);
                mFileSize = ftell((FILE*) mFilePointer);
                fseek((FILE*) mFilePointer, 0, SEEK_SET);
            }
        }
    }

    void ValidContent()
    {
        if(mContent == nullptr)
        {
            ValidSize();
            mContent = new uint08s();
            if(mTypeAssets)
            {
                #if BOSS_WINDOWS
                    fread(mContent->AtDumping(0, mFileSize), 1, mFileSize, (FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #elif BOSS_LINUX
                    fread(mContent->AtDumping(0, mFileSize), 1, mFileSize, (FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #elif BOSS_ANDROID
                    const void* AssetBuffer = AAsset_getBuffer((AAsset*) mFilePointer);
                    Memory::Copy(mContent->AtDumping(0, mFileSize), AssetBuffer, mFileSize);
                #elif BOSS_MAC_OSX || BOSS_IPHONE
                    fread(mContent->AtDumping(0, mFileSize), 1, mFileSize, (FILE*) mFilePointer);
                    fseek((FILE*) mFilePointer, 0, SEEK_SET);
                #else
                    #error 준비되지 않은 플랫폼입니다
                #endif
            }
            else
            {
                fread(mContent->AtDumping(0, mFileSize), 1, mFileSize, (FILE*) mFilePointer);
                fseek((FILE*) mFilePointer, 0, SEEK_SET);
            }
        }
    }

public:
    static String GetParsedFilename(char const* filename)
    {
        String ParsedFilename;
        sint32 SavedSize = 0;
        while(*filename != '\0')
        {
            const char NewChar = *filename;
            if(NewChar == '\\' || NewChar == '/')
            {
                ParsedFilename += '/';
                SavedSize = 0;
            }
            else
            {
                if(NewChar == ':')
                    ParsedFilename = ParsedFilename.Right(SavedSize);
                ParsedFilename += NewChar;
                SavedSize++;
            }
            filename++;
        }
        return ParsedFilename;
    }

public:
    sint32 mFileID;
    bool mTypeAssets;
    void* mFilePointer;
    bool mNeedSave;
    uint08s* mContent;
    long int mFileSize;
    long int mFileOffset;
};
static Map<FileClass> gAllFiles;
static sint32 gLastFileID = -1;

#if BOSS_ANDROID
    JavaVM* g_JVM;
    jint JNI_OnLoad(JavaVM* vm, void*)
    {
        g_JVM = vm;
        return JNI_VERSION_1_6;
    }
    extern jobject GetAndroidApplicationContext();
    static AAssetManager* GetAAssetManager()
    {
        static AAssetManager* pAssetManager = nullptr;
        if(!pAssetManager)
        {
            jobject context = GetAndroidApplicationContext();
            BOSS_TRACE("context=0x%08X", context);

            JNIEnv* env = nullptr;
            BOSS_TRACE("g_JVM=0x%08X", g_JVM);
            g_JVM->AttachCurrentThread(&env, 0);
            jclass contextClass = env->GetObjectClass(context);
            BOSS_TRACE("contextClass=0x%08X", contextClass);
            jmethodID methodGetAssets = env->GetMethodID(contextClass,
                "getAssets", "()Landroid/content/res/AssetManager;");
            BOSS_TRACE("methodGetAssets=0x%08X", methodGetAssets);
            jobject localAssetManager = env->CallObjectMethod(context, methodGetAssets);
            BOSS_TRACE("localAssetManager=0x%08X", localAssetManager);
            jobject globalAssetManager = env->NewGlobalRef(localAssetManager);
            BOSS_TRACE("globalAssetManager=0x%08X", globalAssetManager);
            pAssetManager = AAssetManager_fromJava(env, globalAssetManager);
            BOSS_TRACE("pAssetManager=0x%08X", pAssetManager);
        }
        return pAssetManager;
    }
#endif

extern "C" void* boss_fopen(char const* filename, char const* mode)
{
    const String ParsedFilename = FileClass::GetParsedFilename(filename);
    filename = ParsedFilename;
    const bool AssetsFlag = (filename[0] == 'a' && filename[1] == 's' && filename[2] == 's' &&
        filename[3] == 'e' && filename[4] == 't' && filename[5] == 's' && filename[6] == ':');

    bool ReadFlag = false, SaveFlag = false;
    while(*mode != '\0')
    {
        if(*mode == 'r') ReadFlag = true;
        else if(*mode == '+') SaveFlag = true;
        else if(*mode == 'a')
        {
            BOSS_ASSERT("a모드는 준비중입니다", false);
        }
        mode++;
    }

    if(ReadFlag)
    {
        if(!AssetsFlag)
        {
            FILE* NewFilePointer = fopen(filename, (SaveFlag)? "r+b" : "rb");
            if(!NewFilePointer) return nullptr;
            FileClass* NewFile = &gAllFiles[++gLastFileID];
            NewFile->mFileID = gLastFileID;
            NewFile->mTypeAssets = false;
            NewFile->mFilePointer = NewFilePointer;
            NewFile->mNeedSave = SaveFlag;
            return NewFile;
        }
        else if(!SaveFlag)
        {
            #if BOSS_WINDOWS
                const String FilenameAtAssets = String("../assets") + &filename[7];
                filename = FilenameAtAssets;
                FILE* NewAssetsPointer = fopen(filename, "rb");
                if(!NewAssetsPointer) return nullptr;
            #elif BOSS_LINUX
                const String FilenameAtAssets = String("../assets") + &filename[7];
                filename = FilenameAtAssets;
                FILE* NewAssetsPointer = fopen(filename, "rb");
                if(!NewAssetsPointer) return nullptr;
            #elif BOSS_ANDROID
                const String FilenameAtAssets = &filename[8]; // "assets:/"
                filename = FilenameAtAssets;
                AAsset* NewAssetsPointer = AAssetManager_open(GetAAssetManager(), filename, AASSET_MODE_BUFFER);
                if(!NewAssetsPointer) return nullptr;
            #elif BOSS_MAC_OSX || BOSS_IPHONE
                const String FilenameAtAssets = String("../assets") + &filename[7];
                filename = FilenameAtAssets;
                FILE* NewAssetsPointer = fopen(filename, "rb");
                if(!NewAssetsPointer) return nullptr;
            #else
                #error 준비되지 않은 플랫폼입니다
            #endif
            FileClass* NewFile = &gAllFiles[++gLastFileID];
            NewFile->mFileID = gLastFileID;
            NewFile->mTypeAssets = true;
            NewFile->mFilePointer = NewAssetsPointer;
            NewFile->mNeedSave = false;
            return NewFile;
        }
    }
    else if(!AssetsFlag)
    {
        FILE* NewFilePointer = fopen(filename, "wb");
        if(!NewFilePointer) return nullptr;
        FileClass* NewFile = &gAllFiles[++gLastFileID];
        NewFile->mFileID = gLastFileID;
        NewFile->mTypeAssets = false;
        NewFile->mFilePointer = NewFilePointer;
        NewFile->mNeedSave = true;
        NewFile->mContent = new uint08s();
        NewFile->mFileSize = 0;
        return NewFile;
    }
    return nullptr;
}

extern "C" int boss_fclose(void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        gAllFiles.Remove(CurFile->mFileID);
        return 0;
    }
    return EOF;
}

extern "C" int boss_fseek(void* stream, long int offset, int origin)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        switch(origin)
        {
        case SEEK_SET:
            CurFile->mFileOffset = offset;
            break;
        case SEEK_CUR:
            CurFile->mFileOffset += offset;
            break;
        case SEEK_END:
            CurFile->ValidSize();
            CurFile->mFileOffset = CurFile->mFileSize + offset;
            break;
        }
        return 0;
    }
    return EOF;
}

extern "C" long int boss_ftell(void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
        return CurFile->mFileOffset;
    return EOF;
}

extern "C" size_t boss_fread(void* ptr, size_t size, size_t count, void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        CurFile->ValidContent();
        if(CurFile->mFileOffset < CurFile->mFileSize)
        {
            const sint32 CopyLen = Math::Min(size * count,
                (CurFile->mFileSize - CurFile->mFileOffset) / size * size);
            Memory::Copy(ptr, CurFile->mContent->AtDumping(CurFile->mFileOffset, CopyLen), CopyLen);
            CurFile->mFileOffset += CopyLen;
            return CopyLen / size;
        }
    }
    return 0;
}

extern "C" size_t boss_fwrite(const void* ptr, size_t size, size_t count, void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        CurFile->ValidContent();
        const sint32 CopyLen = size * count;
        Memory::Copy(CurFile->mContent->AtDumping(CurFile->mFileOffset, CopyLen), ptr, CopyLen);
        CurFile->mFileSize = CurFile->mContent->Count();
        CurFile->mFileOffset += CopyLen;
        return CopyLen / size;
    }
    return 0;
}

extern "C" int boss_fgetc(void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        CurFile->ValidContent();
        if(CurFile->mFileOffset < CurFile->mFileSize)
            return (*CurFile->mContent)[(sint32) CurFile->mFileOffset++];
    }
    return EOF;
}

extern "C" int boss_ungetc(int character, void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        CurFile->ValidContent();
        if(0 < CurFile->mFileOffset)
        {
            CurFile->mFileOffset--;
            CurFile->mContent->At(CurFile->mFileOffset) = (uint08) character;
            return character;
        }
    }
    return EOF;
}

extern "C" void boss_rewind(void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
        CurFile->mFileOffset = 0;
}

extern "C" char* boss_fgets(char* str, int num, void* stream)
{
    FileClass* CurFile = (FileClass*) stream;
    if(CurFile)
    {
        CurFile->ValidContent();
        bytes CurContent = CurFile->mContent->AtDumping(0, CurFile->mFileSize);
        if(CurFile->mFileOffset < CurFile->mFileSize)
        {
            for(sint32 i = 0, iend = num - 1; i < iend; ++i)
            {
                const char NewChar = (const char) CurContent[CurFile->mFileOffset++];
                str[i] = NewChar;
                if(NewChar == '\n' || CurFile->mFileOffset == CurFile->mFileSize)
                {
                    str[i + 1] = '\0';
                    return str;
                }
            }
            str[num - 1] = '\0';
            return str;
        }
    }
    return nullptr;
}

class DirClass
{
public:
    DirClass()
    {
        mDirID = -1;
        mTypeAssets = false;
        mDirHandle = nullptr;
        mDirPointer = nullptr;
        mNextFlag = false;
    }

    ~DirClass()
    {
        #if BOSS_WINDOWS
            FindClose((HANDLE) mDirHandle);
            delete (WIN32_FIND_DATAW*) mDirPointer;
        #elif BOSS_LINUX
            closedir((DIR*) mDirHandle);
        #elif BOSS_MAC_OSX || BOSS_IPHONE
            closedir((DIR*) mDirHandle);
        #elif BOSS_ANDROID
            if(mTypeAssets)
                AAssetDir_close((AAssetDir*) mDirHandle);
            else closedir((DIR*) mDirHandle);
        #else
            #error 준비되지 않은 플랫폼입니다
        #endif
    }

public:
    sint32 mDirID;
    bool mTypeAssets;
    void* mDirHandle;
    void* mDirPointer;
    bool mNextFlag;
    String mLastFilePath;
};
static Map<DirClass> gAllDirs;
static sint32 gLastDirID = -1;

extern "C" void* boss_opendir(const char* dirname)
{
    const String ParsedDirname = FileClass::GetParsedFilename(dirname);
    dirname = ParsedDirname;
    const bool AssetsFlag = (dirname[0] == 'a' && dirname[1] == 's' && dirname[2] == 's' &&
        dirname[3] == 'e' && dirname[4] == 't' && dirname[5] == 's' && dirname[6] == ':');

    if(AssetsFlag)
    {
        #if BOSS_WINDOWS
            const String DirnameAtAssets = String("../assets") + &dirname[7];
            dirname = DirnameAtAssets;
            WIN32_FIND_DATAW* NewFindFileData = new WIN32_FIND_DATAW();
            HANDLE NewDirHandle = FindFirstFileW((wchars) WString::FromChars(dirname), NewFindFileData);
            if(NewDirHandle == INVALID_HANDLE_VALUE)
            {
                delete NewFindFileData;
                return nullptr;
            }
        #elif BOSS_LINUX
            const String ParentPath = ".";
            String DirnameAtAssets = &dirname[8]; // "assets:/"
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            DIR* NewDirHandle = opendir((chars) (ParentPath + '/' + dirname));
            if(!NewDirHandle) return nullptr;
            void* NewFindFileData = nullptr;
        #elif BOSS_MAC_OSX || BOSS_IPHONE
            CFURLRef ResourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
            #if BOSS_MAC_OSX
                char TempString[1024];
                if(!CFURLGetFileSystemRepresentation(ResourceURL, TRUE, (UInt8*) TempString, 1024))
                    return nullptr;
                const String ParentPath = TempString;
            #else // 아이폰 및 시뮬레이터
                const String ParentPath = CFStringGetCStringPtr(CFURLGetString(ResourceURL), kCFStringEncodingUTF8) + 7; // 7은 "file://"
            #endif
            CFRelease(ResourceURL);
            String DirnameAtAssets = &dirname[8]; // "assets:/"
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            DIR* NewDirHandle = opendir((chars) (ParentPath + '/' + dirname));
            if(!NewDirHandle) return nullptr;
            void* NewFindFileData = nullptr;
        #elif BOSS_ANDROID
            String DirnameAtAssets = &dirname[8]; // "assets:/"
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            AAssetDir* NewDirHandle = AAssetManager_openDir(GetAAssetManager(), dirname);
            if(!NewDirHandle) return nullptr;
            chars NewFindFileData = AAssetDir_getNextFileName(NewDirHandle);
            if(!NewFindFileData)
            {
                AAssetDir_close(NewDirHandle);
                return nullptr;
            }
        #else
            #error 준비되지 않은 플랫폼입니다
        #endif
        DirClass* NewDir = &gAllDirs[++gLastDirID];
        NewDir->mDirID = gLastDirID;
        NewDir->mTypeAssets = true;
        NewDir->mDirHandle = (void*) NewDirHandle;
        NewDir->mDirPointer = (void*) NewFindFileData;
        NewDir->mNextFlag = true;
        return NewDir;
    }
    else
    {
        #if BOSS_WINDOWS
            WIN32_FIND_DATAW* NewFindFileData = new WIN32_FIND_DATAW();
            HANDLE NewDirHandle = FindFirstFileW((wchars) WString::FromChars(dirname), NewFindFileData);
            if(NewDirHandle == INVALID_HANDLE_VALUE)
            {
                delete NewFindFileData;
                return nullptr;
            }
        #elif BOSS_LINUX
            if(dirname[0] == 'Q' && dirname[1] == ':')
                dirname += 2;
            String DirnameAtAssets = dirname;
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            DIR* NewDirHandle = opendir(dirname);
            if(!NewDirHandle) return nullptr;
            void* NewFindFileData = nullptr;
        #elif BOSS_MAC_OSX || BOSS_IPHONE
            if(dirname[0] == 'Q' && dirname[1] == ':')
                dirname += 2;
            String DirnameAtAssets = dirname;
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            DIR* NewDirHandle = opendir(dirname);
            if(!NewDirHandle) return nullptr;
            void* NewFindFileData = nullptr;
        #elif BOSS_ANDROID
            if(dirname[0] == 'Q' && dirname[1] == ':')
                dirname += 2;
            String DirnameAtAssets = dirname;
            if(2 < DirnameAtAssets.Length() && DirnameAtAssets[-3] == '/' && DirnameAtAssets[-2] == '*')
                DirnameAtAssets.Sub(2);
            dirname = DirnameAtAssets;
            DIR* NewDirHandle = opendir(dirname);
            if(!NewDirHandle) return nullptr;
            void* NewFindFileData = nullptr;
        #else
            #error 준비되지 않은 플랫폼입니다
        #endif
        DirClass* NewDir = &gAllDirs[++gLastDirID];
        NewDir->mDirID = gLastDirID;
        NewDir->mTypeAssets = false;
        NewDir->mDirHandle = (void*) NewDirHandle;
        NewDir->mDirPointer = (void*) NewFindFileData;
        NewDir->mNextFlag = true;
        return NewDir;
    }
    return nullptr;
}

extern "C" void* boss_readdir(void* dir)
{
    DirClass* CurDir = (DirClass*) dir;
    if(CurDir)
    {
        #if BOSS_WINDOWS
            if(CurDir->mNextFlag)
            {
                CurDir->mLastFilePath = String::FromWChars(((WIN32_FIND_DATAW*) CurDir->mDirPointer)->cFileName);
                CurDir->mNextFlag = FindNextFileW((HANDLE) CurDir->mDirHandle, (WIN32_FIND_DATAW*) CurDir->mDirPointer);
                return (void*)(chars) CurDir->mLastFilePath;
            }
        #elif BOSS_LINUX
            struct dirent* CurDirEnt = readdir((DIR*) CurDir->mDirHandle);
            if(CurDirEnt)
            {
                CurDir->mLastFilePath = CurDirEnt->d_name;
                return (void*)(chars) CurDir->mLastFilePath;
            }
        #elif BOSS_MAC_OSX || BOSS_IPHONE
            struct dirent* CurDirEnt = readdir((DIR*) CurDir->mDirHandle);
            if(CurDirEnt)
            {
                CurDir->mLastFilePath = CurDirEnt->d_name;
                return (void*)(chars) CurDir->mLastFilePath;
            }
        #elif BOSS_ANDROID
            if(CurDir->mTypeAssets)
            {
                if(CurDir->mNextFlag)
                {
                    CurDir->mLastFilePath = (chars) CurDir->mDirPointer;
                    CurDir->mDirPointer = (void*) AAssetDir_getNextFileName((AAssetDir*) CurDir->mDirHandle);
                    CurDir->mNextFlag = (CurDir->mDirPointer != nullptr);
                    return (void*)(chars) CurDir->mLastFilePath;
                }
            }
            else
            {
                struct dirent* CurDirEnt = readdir((DIR*) CurDir->mDirHandle);
                if(CurDirEnt)
                {
                    CurDir->mLastFilePath = CurDirEnt->d_name;
                    return (void*)(chars) CurDir->mLastFilePath;
                }
            }
        #else
            #error 준비되지 않은 플랫폼입니다
        #endif
    }
    return nullptr;
}

const char* boss_getdirname(void* dirent)
{
    return (chars) dirent;
}

extern "C" int boss_closedir(void* dir)
{
    DirClass* CurDir = (DirClass*) dir;
    if(CurDir)
    {
        gAllDirs.Remove(CurDir->mDirID);
        return 0;
    }
    return 1;
}
