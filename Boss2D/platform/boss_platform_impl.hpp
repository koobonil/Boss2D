#pragma once
#include <platform/boss_platform.hpp>

namespace BOSS
{
    namespace PlatformImpl
    {
        // 일반함수와 전역인스턴스
        namespace Core
        {
            sint32 GetProcedureCount();
            ProcedureCB GetProcedureCB(sint32 i);
            payload GetProcedureData(sint32 i);
            String NormalPath(chars itemname, bool QCodeTest = true);
            WString NormalPathW(wchars itemname, bool QCodeTest = true);
            template<typename TYPE, datatype DATATYPE>
            TYPE* GetManagerBy(sint32 id)
            {
                buffer Result = GetManager(id);
                const bool IsResultEnabled = Buffer::TypeCheck<TYPE, DATATYPE>(Result);
                BOSS_ASSERT("Manager의 Type이 일치하지 않습니다", IsResultEnabled);
                return (TYPE*) Result;
            }

            extern View::CreatorCB g_Creator;
            extern String g_AssetsRoot;
            extern String g_AssetsRemRoot;
        }

        // 래핑함수
        namespace Wrap
        {
            void AddWindowProcedure(WindowEvent event, ProcedureCB cb, payload data);

            chars Utility_GetOSName();
            sint64 Utility_CurrentAvailableMemory(sint64* totalbytes);

            void Option_SetOptionFlag(chars name, bool flag);
            bool Option_GetOptionFlag(chars name);
            Strings Option_GetOptionFlagNames();
            void Option_SetOptionText(chars name, chars text);
            chars Option_GetOptionText(chars name);
            Strings Option_GetOptionTextNames();
            void Option_SetOptionPayload(chars name, payload data);
            payload Option_GetOptionPayload(chars name);
            Strings Option_GetOptionPayloadNames();

            bool Popup_FileDialog(String& path, String* shortpath, chars title, bool isdir);
            sint32 Popup_MessageDialog(chars title, chars text, DialogButtonType type);
            void Popup_WebBrowserDialog(String url);
            void Popup_ProgramDialog(chars path, chars args);

            WString File_GetDirName(wchars itemname, wchar_t badslash, wchar_t goodslash);
            WString File_GetShortName(wchars itemname);
            sint32 File_GetDriveCode();
            bool File_Tempname(char* format, sint32 length);
            void File_ResetAssetsRoot(chars dirname);
            void File_ResetAssetsRemRoot(chars dirname);

            h_web_native Web_CreateNative(chars url, bool clearcookies, Platform::Web::EventCB cb, payload data);
            void Web_ReleaseNative(h_web_native web_native);
            void Web_ReloadNative(h_web_native web_native, chars url);
        }
    }
}
