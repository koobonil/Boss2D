#include <boss.h>

#if !defined(BOSS_NEED_ADDON_SSL) || (BOSS_NEED_ADDON_SSL != 0 && BOSS_NEED_ADDON_SSL != 1)
    #error BOSS_NEED_ADDON_SSL macro is invalid use
#endif
bool __LINK_ADDON_SSL__() {return true;} // 링크옵션 /OPT:NOREF가 안되서 임시코드

#if BOSS_NEED_ADDON_SSL

#include "boss_addon_ssl.hpp"

extern "C"
{
    #include <addon/openssl-1.1.0c_for_boss/include/openssl/md5.h>
}

#include <boss.hpp>
#include <platform/boss_platform.hpp>

// 등록과정
namespace BOSS
{
    BOSS_DECLARE_ADDON_FUNCTION(Ssl, ToMD5, chars, bytes, sint32)

    static autorun Bind_AddOn_Ssl()
    {
        Core_AddOn_Ssl_ToMD5() = Customized_AddOn_Ssl_ToMD5;
        return true;
    }
    static autorun _ = Bind_AddOn_Ssl();
}

// 구현부
namespace BOSS
{
    chars Customized_AddOn_Ssl_ToMD5(bytes binary, sint32 length)
    {
        unsigned char Digest[MD5_DIGEST_LENGTH];
        MD5(binary, length, Digest);

        static char Result[33];
        for(sint32 i = 0; i < 16; ++i)
            boss_snprintf(&Result[2 * i], 3, "%02x", (uint32) Digest[i]);
        return Result;
    }
}

#endif
