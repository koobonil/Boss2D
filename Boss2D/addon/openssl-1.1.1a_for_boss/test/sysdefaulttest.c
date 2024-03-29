/*
 * Copyright 2016-2018 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include BOSS_OPENSSL_V_openssl__opensslconf_h //original-code:<openssl/opensslconf.h>

#include <string.h>
#include BOSS_OPENSSL_V_openssl__evp_h //original-code:<openssl/evp.h>
#include BOSS_OPENSSL_V_openssl__ssl_h //original-code:<openssl/ssl.h>
#include BOSS_OPENSSL_V_openssl__tls1_h //original-code:<openssl/tls1.h>
#include "testutil.h"

static SSL_CTX *ctx;

static int test_func(void)
{
    if (!TEST_int_eq(SSL_CTX_get_min_proto_version(ctx), TLS1_2_VERSION)
        && !TEST_int_eq(SSL_CTX_get_max_proto_version(ctx), TLS1_2_VERSION)) {
        TEST_info("min/max version setting incorrect");
        return 0;
    }
    return 1;
}

int global_init(void)
{
    if (!OPENSSL_init_ssl(OPENSSL_INIT_ENGINE_ALL_BUILTIN
                          | OPENSSL_INIT_LOAD_CONFIG, NULL))
        return 0;
    return 1;
}

int setup_tests(void)
{
    if (!TEST_ptr(ctx = SSL_CTX_new(TLS_method())))
        return 0;
    ADD_TEST(test_func);
    return 1;
}

void cleanup_tests(void)
{
    SSL_CTX_free(ctx);
}
