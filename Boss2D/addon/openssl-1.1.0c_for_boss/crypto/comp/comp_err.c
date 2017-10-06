/*
 * Generated by util/mkerr.pl DO NOT EDIT
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include BOSS_OPENSSL_V_openssl__err_h //original-code:<openssl/err.h>
#include BOSS_OPENSSL_V_openssl__comp_h //original-code:<openssl/comp.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

# define ERR_FUNC(func) ERR_PACK(ERR_LIB_COMP,func,0)
# define ERR_REASON(reason) ERR_PACK(ERR_LIB_COMP,0,reason)

static ERR_STRING_DATA COMP_str_functs[] = {
    {ERR_FUNC(COMP_F_BIO_ZLIB_FLUSH), "bio_zlib_flush"},
    {ERR_FUNC(COMP_F_BIO_ZLIB_NEW), "bio_zlib_new"},
    {ERR_FUNC(COMP_F_BIO_ZLIB_READ), "bio_zlib_read"},
    {ERR_FUNC(COMP_F_BIO_ZLIB_WRITE), "bio_zlib_write"},
    {0, NULL}
};

static ERR_STRING_DATA COMP_str_reasons[] = {
    {ERR_REASON(COMP_R_ZLIB_DEFLATE_ERROR), "zlib deflate error"},
    {ERR_REASON(COMP_R_ZLIB_INFLATE_ERROR), "zlib inflate error"},
    {ERR_REASON(COMP_R_ZLIB_NOT_SUPPORTED), "zlib not supported"},
    {0, NULL}
};

#endif

int ERR_load_COMP_strings(void)
{
#ifndef OPENSSL_NO_ERR

    if (ERR_func_error_string(COMP_str_functs[0].error) == NULL) {
        ERR_load_strings(0, COMP_str_functs);
        ERR_load_strings(0, COMP_str_reasons);
    }
#endif
    return 1;
}
