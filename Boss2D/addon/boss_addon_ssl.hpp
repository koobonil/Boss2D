#pragma once
#include <boss.h>

extern "C"
{
    #include "boss_integration_openssl-1.1.0c.h"
    #include <addon/openssl-1.1.0c_for_boss/include/openssl/md5.h>
}

#include <boss.hpp>

namespace BOSS
{
    class MD5Class
    {
    public:
        MD5_CTX mCTX;
        unsigned char mDigest[MD5_DIGEST_LENGTH];
    };
}
