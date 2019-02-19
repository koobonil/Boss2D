#include <boss.h>

#if !defined(BOSS_NEED_ADDON_SSL) || (BOSS_NEED_ADDON_SSL != 0 && BOSS_NEED_ADDON_SSL != 1)
    #error BOSS_NEED_ADDON_SSL macro is invalid use
#endif
bool __LINK_ADDON_SSL__() {return true;} // 링크옵션 /OPT:NOREF가 안되서 임시코드

#if BOSS_NEED_ADDON_SSL

#include "boss_addon_ssl.hpp"

extern "C"
{
    #include <addon/openssl-1.1.1a_for_boss/include/openssl/crypto.h>
}

#include <boss.hpp>
#include <platform/boss_platform.hpp>

// 등록과정
namespace BOSS
{
    BOSS_DECLARE_ADDON_FUNCTION(Ssl, CreateMD5, id_md5, void)
    BOSS_DECLARE_ADDON_FUNCTION(Ssl, ReleaseMD5, chars, id_md5)
    BOSS_DECLARE_ADDON_FUNCTION(Ssl, UpdateMD5, void, id_md5, bytes, sint32)
    BOSS_DECLARE_ADDON_FUNCTION(Ssl, ToMD5, chars, bytes, sint32)

    static autorun Bind_AddOn_Ssl()
    {
        Core_AddOn_Ssl_CreateMD5() = Customized_AddOn_Ssl_CreateMD5;
        Core_AddOn_Ssl_ReleaseMD5() = Customized_AddOn_Ssl_ReleaseMD5;
        Core_AddOn_Ssl_UpdateMD5() = Customized_AddOn_Ssl_UpdateMD5;
        Core_AddOn_Ssl_ToMD5() = Customized_AddOn_Ssl_ToMD5;
        return true;
    }
    static autorun _ = Bind_AddOn_Ssl();
}

// 구현부
namespace BOSS
{
    id_md5 Customized_AddOn_Ssl_CreateMD5(void)
    {
        MD5Class* NewMD5 = new MD5Class();

        if(!MD5_Init(&NewMD5->mCTX))
        {
            delete NewMD5;
            return nullptr;
        }

        return (id_md5) NewMD5;
    }

    chars Customized_AddOn_Ssl_ReleaseMD5(id_md5 md5)
    {
        MD5Class* OldMD5 = (MD5Class*) md5;
        if(!OldMD5) return "";

        MD5_Final(OldMD5->mDigest, &OldMD5->mCTX);
        OPENSSL_cleanse(&OldMD5->mCTX, sizeof(OldMD5->mCTX));

        static char Result[2 * MD5_DIGEST_LENGTH + 1];
        for(sint32 i = 0; i < MD5_DIGEST_LENGTH; ++i)
            boss_snprintf(&Result[2 * i], 3, "%02x", (uint32) OldMD5->mDigest[i]);
        delete OldMD5;
        return Result;
    }

    void Customized_AddOn_Ssl_UpdateMD5(id_md5 md5, bytes binary, sint32 length)
    {
        MD5Class* CurMD5 = (MD5Class*) md5;
        if(!CurMD5) return;

        MD5_Update(&CurMD5->mCTX, binary, length);
    }

    chars Customized_AddOn_Ssl_ToMD5(bytes binary, sint32 length)
    {
        unsigned char Digest[MD5_DIGEST_LENGTH];
        MD5(binary, length, Digest);

        static char Result[2 * MD5_DIGEST_LENGTH + 1];
        for(sint32 i = 0; i < MD5_DIGEST_LENGTH; ++i)
            boss_snprintf(&Result[2 * i], 3, "%02x", (uint32) Digest[i]);
        return Result;
    }
}

#endif
