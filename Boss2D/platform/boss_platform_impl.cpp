#include <boss.hpp>
#include "boss_platform_impl.hpp"

#if BOSS_WINDOWS
    #include <shlobj.h>
    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "comdlg32.lib")
    #pragma comment(lib, "ole32.lib")
    #pragma comment(lib, "shell32.lib")
#elif BOSS_LINUX
    #include <gtk/gtk.h>
#elif BOSS_IPHONE
    #include "ios/src/BossWebView.h"
#elif BOSS_ANDROID
    #include <sys/sysinfo.h>
    #include <jni.h>
#endif

#if BOSS_IPHONE
    extern void* GetIOSApplicationUIView();
#elif BOSS_ANDROID
    extern JNIEnv* GetAndroidJNIEnv();
    extern jobject GetAndroidApplicationActivity();
#endif

namespace BOSS
{
    namespace PlatformImpl
    {
        ////////////////////////////////////////////////////////////////////////////////
        // 일반함수와 전역인스턴스
        ////////////////////////////////////////////////////////////////////////////////
        namespace Core
        {
            class ProcedureClass
            {
                BOSS_DECLARE_NONCOPYABLE_CLASS(ProcedureClass)
            public:
                ProcedureClass() {mCb = nullptr; mData = nullptr;}
                ~ProcedureClass() {}
            public:
                ProcedureCB mCb;
                payload mData;
            };
            Array<ProcedureClass, datatype_pod_canmemcpy> g_AllProcedures;

            sint32 GetProcedureCount()
            {
                return g_AllProcedures.Count();
            }

            ProcedureCB GetProcedureCB(sint32 i)
            {
                return g_AllProcedures[i].mCb;
            }

            payload GetProcedureData(sint32 i)
            {
                return g_AllProcedures[i].mData;
            }

            String NormalPath(chars itemname, bool QCodeTest)
            {
                chars NormalName = boss_normalpath(itemname, nullptr);
                if(QCodeTest && NormalName[0] == 'Q' && NormalName[1] == ':')
                    NormalName += 2;
                return String(NormalName);
            }

            WString NormalPathW(wchars itemname, bool QCodeTest)
            {
                String ItemName = String::FromWChars(itemname);
                chars NormalName = boss_normalpath(ItemName, nullptr);
                if(QCodeTest && NormalName[0] == 'Q' && NormalName[1] == ':')
                    NormalName += 2;
                return WString::FromChars(NormalName);
            }

            String& AssetsRoot()
            {
                return *BOSS_STORAGE_SYS(String);
            }

            String& AssetsRemRoot()
            {
                return *BOSS_STORAGE_SYS(String);
            }

            View::CreatorCB g_Creator = View::Creator;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // 래핑함수
        ////////////////////////////////////////////////////////////////////////////////
        namespace Wrap
        {
            void AddWindowProcedure(WindowEvent event, ProcedureCB cb, payload data)
            {
                auto& NewProcedure = Core::g_AllProcedures.AtAdding();
                NewProcedure.mCb = cb;
                NewProcedure.mData = data;
            }

            chars Utility_GetOSName()
            {
                #if BOSS_WINDOWS
                    return "WINDOWS";
                #elif BOSS_LINUX
                    return "UBUNTU";
                #elif BOSS_MAC_OSX
                    return "OSX";
                #elif BOSS_IPHONE
                    return "IOS";
                #elif BOSS_ANDROID
                    return "ANDROID";
                #else
                    return "UNKNOWN";
                #endif
            }

            sint64 Utility_CurrentAvailableMemory(sint64* totalbytes)
            {
                #if BOSS_WINDOWS
                    MEMORYSTATUSEX MemoryStatus;
                    Memory::Set(&MemoryStatus, 0, sizeof(MemoryStatus));
                    MemoryStatus.dwLength = sizeof(MemoryStatus);
                    if(GlobalMemoryStatusEx(&MemoryStatus))
                    {
                        if(totalbytes) *totalbytes = (sint64) MemoryStatus.ullTotalPhys;
                        return (sint64) MemoryStatus.ullAvailPhys;
                    }
                #elif BOSS_ANDROID
                    struct sysinfo MemoryStatus;
                    if(!sysinfo(&MemoryStatus))
                    {
                        if(totalbytes) *totalbytes = (sint64) MemoryStatus.totalram;
                        return (sint64) MemoryStatus.freeram;
                    }
                #endif
                return -1;
            }

            class StaticalMutexClass
            {
            public:
                StaticalMutexClass()
                {
                    // static 인스턴스끼리의 호출순서로 생성자보다 Lock()이 먼저 호출될 수 있기에
                    // 본 생성자에선 mMutex를 초기화하지 않는다. (static 인스턴스는 자동으로 nullptr초기화됨)
                }
                ~StaticalMutexClass()
                {
                    if(mMutex)
                    {
                        Mutex::Close(mMutex);
                        mMutex = nullptr;
                    }
                }
            public:
                inline void Lock()
                {
                    if(!mMutex) // static 인스턴스끼리의 호출로 Lock()이 생성자보다 먼저 호출될 수 있음.
                        mMutex = Mutex::Open();
                    Mutex::Lock(mMutex);
                }
                inline void Unlock()
                {
                    if(mMutex)
                        Mutex::Unlock(mMutex);
                }
            private:
                id_mutex mMutex;
            };

            static Map<bool> g_OptionFlagMap;
            static StaticalMutexClass g_OptionFlagMutex;
            void Option_SetOptionFlag(chars name, bool flag)
            {
                g_OptionFlagMutex.Lock();
                g_OptionFlagMap(name) = flag;
                g_OptionFlagMutex.Unlock();
            }

            bool Option_GetOptionFlag(chars name)
            {
                g_OptionFlagMutex.Lock();
                bool* Result = g_OptionFlagMap.Access(name);
                g_OptionFlagMutex.Unlock();
                return (Result)? *Result : false;
            }

            Strings Option_GetOptionFlagNames()
            {
                Strings Result;
                payload Param = (payload) &Result;
                g_OptionFlagMutex.Lock();
                g_OptionFlagMap.AccessByCallback(
                    [](const MapPath* path, bool* data, payload param)->void
                    {
                        Strings& Result = *((Strings*) param);
                        Result.AtAdding() = &path->GetPath()[0];
                    }, Param);
                g_OptionFlagMutex.Unlock();
                return Result;
            }

            static Map<String> g_OptionTextMap;
            static StaticalMutexClass g_OptionTextMutex;
            void Option_SetOptionText(chars name, chars text)
            {
                g_OptionTextMutex.Lock();
                g_OptionTextMap(name) = text;
                g_OptionTextMutex.Unlock();
            }

            chars Option_GetOptionText(chars name)
            {
                g_OptionTextMutex.Lock();
                String* Result = g_OptionTextMap.Access(name);
                g_OptionTextMutex.Unlock();
                return (Result)? *Result : "";
            }

            Strings Option_GetOptionTextNames()
            {
                Strings Result;
                payload Param = (payload) &Result;
                g_OptionTextMutex.Lock();
                g_OptionTextMap.AccessByCallback(
                    [](const MapPath* path, String* data, payload param)->void
                    {
                        Strings& Result = *((Strings*) param);
                        Result.AtAdding() = &path->GetPath()[0];
                    }, Param);
                g_OptionTextMutex.Unlock();
                return Result;
            }

            static Map<payload> g_OptionPayloadMap;
            static StaticalMutexClass g_OptionPayloadMutex;
            void Option_SetOptionPayload(chars name, payload data)
            {
                g_OptionPayloadMutex.Lock();
                g_OptionPayloadMap(name) = data;
                g_OptionPayloadMutex.Unlock();
            }

            payload Option_GetOptionPayload(chars name)
            {
                g_OptionPayloadMutex.Lock();
                payload* Result = g_OptionPayloadMap.Access(name);
                g_OptionPayloadMutex.Unlock();
                return (Result)? *Result : nullptr;
            }

            Strings Option_GetOptionPayloadNames()
            {
                Strings Result;
                payload Param = (payload) &Result;
                g_OptionPayloadMutex.Lock();
                g_OptionPayloadMap.AccessByCallback(
                    [](const MapPath* path, payload* data, payload param)->void
                    {
                        Strings& Result = *((Strings*) param);
                        Result.AtAdding() = &path->GetPath()[0];
                    }, Param);
                g_OptionPayloadMutex.Unlock();
                return Result;
            }

            bool Popup_FileDialog(String& path, String* shortpath, chars title, bool isdir)
            {
                bool Result = false;
                #if BOSS_WINDOWS && defined(_MSC_VER)
                    CoInitialize(NULL);
                    wchar_t ResultPath[_MAX_PATH] = {L'\0'};
                    WString TitleName = WString::FromChars(title);
                    WString InitDir = Platform::File::GetDirName(WString::FromChars(path), L'/', L'\\');

                    if(isdir)
                    {
                        BROWSEINFOW bi = {0};
                        bi.lpszTitle = (wchars) TitleName;
                        bi.lParam = (LPARAM)(wchars) InitDir;
                        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
                        bi.lpfn = [](HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)->int
                            {
                                switch(uMsg)
                                {
                                case BFFM_INITIALIZED:
                                    if(lpData) SendMessageW(hwnd, BFFM_SETSELECTION, TRUE, lpData);
                                    break;
                                }
                                return 0;
                            };

                        LPITEMIDLIST pidlFolder = SHBrowseForFolderW(&bi);
                        if(pidlFolder && SHGetPathFromIDListW(pidlFolder, ResultPath))
                        {
                            CoTaskMemFree(pidlFolder);
                            Result = true;
                        }
                    }
                    else
                    {
                        OPENFILENAMEW ofn = {0};
                        ofn.lStructSize = sizeof(OPENFILENAMEW);
                        ofn.lpstrTitle = (wchars) TitleName;
                        ofn.lpstrInitialDir = (wchars) InitDir;
                        ofn.lpstrFilter =
                            L"All File(*.*)\0"
                            L"*.*\0"
                            L"Text File\0"
                            L"*.txt;*.doc\0";
                        ofn.lpstrFile = ResultPath;
                        ofn.nMaxFile = _MAX_PATH - 4;

                        if(GetOpenFileNameW(&ofn))
                            Result = true;
                    }

                    if(Result)
                    {
                        path = String::FromWChars(ResultPath);
                        if(shortpath) *shortpath = String::FromWChars(Platform::File::GetShortName(ResultPath));
                    }
                    CoUninitialize();
                #elif BOSS_LINUX
                    auto NewDialog = gtk_file_chooser_dialog_new(title, nullptr,
                        (isdir)? GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : GTK_FILE_CHOOSER_ACTION_SAVE,
                        "_Cancel", GTK_RESPONSE_CANCEL, "_OK", GTK_RESPONSE_ACCEPT, nullptr);
                    if(gtk_dialog_run(GTK_DIALOG(NewDialog)) == GTK_RESPONSE_ACCEPT)
                        Result = true;

                    if(Result)
                    {
                        auto CurChooser = GTK_FILE_CHOOSER(NewDialog);
                        auto NewFilename = gtk_file_chooser_get_filename(CurChooser);
                        path = NewFilename;
                        if(shortpath) *shortpath = String::FromWChars(
                            Platform::File::GetShortName(WString::FromChars(NewFilename)));
                        g_free(NewFilename);
                    }
                    gtk_widget_destroy(NewDialog);
                #endif
                return Result;
            }

            sint32 Popup_MessageDialog(chars title, chars text, DialogButtonType type)
            {
                #if BOSS_LINUX
                    GtkMessageType MessageType = GTK_MESSAGE_INFO; // DBT_Ok
                    GtkButtonsType ButtonsType = GTK_BUTTONS_OK;
                    switch(type)
                    {
                    case DBT_YesNo:
                        MessageType = GTK_MESSAGE_QUESTION;
                        ButtonsType = GTK_BUTTONS_YES_NO;
                        break;
                    case DBT_OKCancel:
                        MessageType = GTK_MESSAGE_INFO;
                        ButtonsType = GTK_BUTTONS_OK_CANCEL;
                        break;
                    case DBT_OkCancelIgnore:
                        MessageType = GTK_MESSAGE_WARNING;
                        ButtonsType = GTK_BUTTONS_OK_CANCEL;
                        break;
                    }

                    auto NewDialog = gtk_message_dialog_new(nullptr,
                        GTK_DIALOG_DESTROY_WITH_PARENT, MessageType, ButtonsType, text);
                    gtk_window_set_title(GTK_WINDOW(NewDialog), title);
                    gint Result = gtk_dialog_run(GTK_DIALOG(NewDialog));
                    gtk_widget_destroy(GTK_WIDGET(NewDialog));
                    switch(type)
                    {
                    case DBT_YES_NO:
                        switch(Result)
                        {
                        case GTK_RESPONSE_YES:
                            return 0;
                        case GTK_RESPONSE_NO:
                            return 1;
                        }
                        break;
                    case DBT_OK:
                    case DBT_OK_CANCEL:
                        switch(Result)
                        {
                        case GTK_RESPONSE_OK:
                            return 0;
                        case GTK_RESPONSE_CANCEL:
                            return 1;
                        }
                        break;
                    case DBT_OK_CANCEL_IGNORE:
                        switch(Result)
                        {
                        case GTK_RESPONSE_OK:
                            return 0;
                        case GTK_RESPONSE_CANCEL:
                            return 1;
                        case GTK_RESPONSE_DELETE_EVENT:
                            return 2;
                        }
                        break;
                    }
                #endif
                return -1;
            }

            void Popup_WebBrowserDialog(String url)
            {
                #if BOSS_WINDOWS
                    #ifdef UNICODE
                        ShellExecute(NULL, L"open", (wchars) WString::FromChars(url),
                            NULL, NULL, SW_SHOWNORMAL);
                    #else
                        ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
                    #endif
                #endif
            }

            void Popup_ProgramDialog(chars exepath, chars args, bool admin)
            {
                #if BOSS_WINDOWS
                    WString ExePathW = WString::FromChars(exepath);
                    WString ArgsW = WString::FromChars(args);

                    SHELLEXECUTEINFOW ExecuteInfo;
                    Memory::Set(&ExecuteInfo, 0, sizeof(ExecuteInfo));
                    ExecuteInfo.cbSize = sizeof(ExecuteInfo);
                    ExecuteInfo.fMask = 0;
                    ExecuteInfo.hwnd = NULL;
                    ExecuteInfo.lpVerb = (!admin)? NULL : L"runas";
                    ExecuteInfo.lpFile = ExePathW;
                    ExecuteInfo.lpParameters = ArgsW;
                    ExecuteInfo.lpDirectory = NULL;
                    ExecuteInfo.nShow = SW_SHOWNORMAL;
                    ShellExecuteExW(&ExecuteInfo);
                #endif
            }

            WString File_GetDirName(wchars itemname, wchar_t badslash, wchar_t goodslash)
            {
                if(itemname && itemname[0] != L'\0')
                {
                    String ItemnameUTF8 = String::FromWChars(itemname);
                    ItemnameUTF8.Replace(badslash, goodslash);
                    if(ItemnameUTF8[-2] == goodslash)
                        ItemnameUTF8.Sub(1);

                    if(Platform::File::ExistForDir(ItemnameUTF8))
                        return WString::FromChars(ItemnameUTF8);

                    for(sint32 i = ItemnameUTF8.Length() - 1; 0 <= i; --i)
                        if(ItemnameUTF8[i] == goodslash)
                            return WString::FromChars(ItemnameUTF8, i);
                }
                return L"";
            }

            WString File_GetShortName(wchars itemname)
            {
                if(itemname && itemname[0] != L'\0')
                {
                    String ItemnameUTF8 = String::FromWChars(itemname);
                    if(ItemnameUTF8[-2] == L'/' || ItemnameUTF8[-2] == L'\\')
                        ItemnameUTF8.Sub(1);

                    for(sint32 i = ItemnameUTF8.Length() - 1; 0 <= i; --i)
                        if(ItemnameUTF8[i] == L'/' || ItemnameUTF8[i] == L'\\')
                            return WString::FromChars(&((chars) ItemnameUTF8)[i + 1]);
                }
                return L"";
            }

            sint32 File_GetDriveCode()
            {
                static const sint32 DriveCode = Platform::File::GetFullPath((wchars) L"")[0] - L'A' + 1;
                return DriveCode;
            }

            bool File_Tempname(char* format, sint32 length)
            {
                static const char TempChars[36] = {
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                    'u', 'v', 'w', 'x', 'y', 'z'};

                String NewName;
                do
                {
                    NewName.Empty();
                    for(sint32 i = 0; i < length; ++i)
                    {
                        if(format[i] == 'X')
                            NewName += TempChars[Platform::Utility::Random() % 36];
                        else NewName += format[i];
                    }
                }
                while(Platform::File::CanAccess(WString::FromChars(NewName)));

                Memory::Copy(format, (chars) NewName, length);
                return true;
            }

            void File_ResetAssetsRoot(chars dirname)
            {
                PlatformImpl::Core::AssetsRoot() = dirname;
                PlatformImpl::Core::AssetsRoot() += '/';
            }

            void File_ResetAssetsRemRoot(chars dirname)
            {
                PlatformImpl::Core::AssetsRemRoot() = dirname;
                PlatformImpl::Core::AssetsRemRoot() += '/';
            }

            class WebNativePrivate
            {
            public:
                WebNativePrivate(sint32 id = -1) {mID = id;}
                ~WebNativePrivate()
                {
                    if(mID != -1)
                    {
                        #if BOSS_IPHONE
                            BossWebView::Release(mID);
                        #elif BOSS_ANDROID
                            JNIEnv* env = GetAndroidJNIEnv();
                            jclass BossWebViewClass = env->FindClass("com/boss2d/BossWebView");
                            jmethodID ReleaseMethodID = env->GetStaticMethodID(BossWebViewClass, "Release", "(I)V");
                            env->CallStaticVoidMethod(BossWebViewClass, ReleaseMethodID, mID);
                            env->DeleteLocalRef(BossWebViewClass);
                        #endif
                    }
                }
            public:
                void Reload(chars url)
                {
                }
            private:
                sint32 mID;
            };

            #if BOSS_ANDROID
                class WebPayloadForAndroid
                {
                public:
                    WebPayloadForAndroid() {mCB = nullptr; mData = nullptr;}
                    ~WebPayloadForAndroid() {}
                public:
                    static WebPayloadForAndroid& ST(sint32 id)
                    {static Map<WebPayloadForAndroid> _; return _[id];}
                public:
                    Platform::Web::EventCB mCB;
                    payload mData;
                public:
                    static void OnEvent(JNIEnv* env, jobject obj, jint id, jstring type, jstring text)
                    {
                        chars PrmType = (chars) ((JNIEnv*) env)->GetStringUTFChars(type, nullptr);
                        chars PrmText = (chars) ((JNIEnv*) env)->GetStringUTFChars(text, nullptr);
                        auto& CurPayload = ST(id);
                        CurPayload.mCB(CurPayload.mData, PrmType, PrmText);
                        ((JNIEnv*) env)->ReleaseStringUTFChars(type, PrmType);
                        ((JNIEnv*) env)->ReleaseStringUTFChars(text, PrmText);
                    }
                };
            #endif

            h_web_native Web_CreateNative(chars url, bool clearcookies, Platform::Web::EventCB cb, payload data)
            {
                sint32 ID = -1;
                #if BOSS_WINDOWS
                    Platform::Popup::WebBrowserDialog(url);
                #elif BOSS_IPHONE
                    ID = BossWebView::Create(GetIOSApplicationUIView(), url, clearcookies, cb, data);
                #elif BOSS_ANDROID
                    JNIEnv* env = GetAndroidJNIEnv();
                    jclass BossWebViewClass = env->FindClass("com/boss2d/BossWebView");
                    static bool JustOnce = true;
                    if(JustOnce)
                    {
                        JustOnce = false;
                        JNINativeMethod methods[] {
                            {"OnEvent", "(ILjava/lang/String;Ljava/lang/String;)V", reinterpret_cast<void*>(WebPayloadForAndroid::OnEvent)}};
                        env->RegisterNatives(BossWebViewClass, methods, sizeof(methods) / sizeof(methods[0]));
                    }

                    jmethodID CreateMethodID = env->GetStaticMethodID(BossWebViewClass,
                        "Create", "(Landroid/app/Activity;Ljava/lang/String;Z)I");
                    jobject activity = GetAndroidApplicationActivity();
                    jstring string = env->NewStringUTF(url);
                    ID = env->CallStaticIntMethod(BossWebViewClass, CreateMethodID, activity, string, (jboolean) clearcookies);
                    env->DeleteLocalRef(BossWebViewClass);

                    auto& NewPayload = WebPayloadForAndroid::ST(ID);
                    NewPayload.mCB = cb;
                    NewPayload.mData = data;
                #endif

                auto NewWeb = (WebNativePrivate*) Buffer::AllocNoConstructorOnce<WebNativePrivate>(BOSS_DBG 1);
                BOSS_CONSTRUCTOR(NewWeb, 0, WebNativePrivate, ID);
                auto NewWebHandle = h_web_native::create_by_buf(BOSS_DBG (buffer) NewWeb);
                return NewWebHandle;
            }

            void Web_ReleaseNative(h_web_native web_native)
            {
                web_native.set_buf(nullptr);
            }

            void Web_ReloadNative(h_web_native web_native, chars url)
            {
                if(auto CurWeb = (WebNativePrivate*) web_native.get())
                    CurWeb->Reload(url);
            }
        }
    }
}
