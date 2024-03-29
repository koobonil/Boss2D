/*
 * Copyright 2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include BOSS_OPENSSL_V_openssl__bio_h //original-code:<openssl/bio.h>
#include BOSS_OPENSSL_V_openssl__crypto_h //original-code:<openssl/crypto.h>
#include BOSS_OPENSSL_V_openssl__ssl_h //original-code:<openssl/ssl.h>
#include BOSS_OPENSSL_V_openssl__err_h //original-code:<openssl/err.h>

#include "ssltestlib.h"
#include "testutil.h"

static char *cert = NULL;
static char *privkey = NULL;

#define NUM_TESTS   2


#define DUMMY_CERT_STATUS_LEN  12

static unsigned char certstatus[] = {
    SSL3_RT_HANDSHAKE, /* Content type */
    0xfe, 0xfd, /* Record version */
    0, 1, /* Epoch */
    0, 0, 0, 0, 0, 0x0f, /* Record sequence number */
    0, DTLS1_HM_HEADER_LENGTH + DUMMY_CERT_STATUS_LEN - 2,
    SSL3_MT_CERTIFICATE_STATUS, /* Cert Status handshake message type */
    0, 0, DUMMY_CERT_STATUS_LEN, /* Message len */
    0, 5, /* Message sequence */
    0, 0, 0, /* Fragment offset */
    0, 0, DUMMY_CERT_STATUS_LEN - 2, /* Fragment len */
    0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80 /* Dummy data */
};

#define RECORD_SEQUENCE 10

static int test_dtls_unprocessed(int testidx)
{
    SSL_CTX *sctx = NULL, *cctx = NULL;
    SSL *serverssl1 = NULL, *clientssl1 = NULL;
    BIO *c_to_s_fbio, *c_to_s_mempacket;
    int testresult = 0;

    printf("Starting Test %d\n", testidx);

    if (!create_ssl_ctx_pair(DTLS_server_method(), DTLS_client_method(), &sctx,
                             &cctx, cert, privkey)) {
        printf("Unable to create SSL_CTX pair\n");
        return 0;
    }

    if (!SSL_CTX_set_cipher_list(cctx, "AES128-SHA")) {
        printf("Failed setting cipher list\n");
    }

    c_to_s_fbio = BIO_new(bio_f_tls_dump_filter());
    if (c_to_s_fbio == NULL) {
        printf("Failed to create filter BIO\n");
        goto end;
    }

    /* BIO is freed by create_ssl_connection on error */
    if (!create_ssl_objects(sctx, cctx, &serverssl1, &clientssl1, NULL,
                               c_to_s_fbio)) {
        printf("Unable to create SSL objects\n");
        ERR_print_errors_fp(stdout);
        goto end;
    }

    if (testidx == 1)
        certstatus[RECORD_SEQUENCE] = 0xff;

    /*
     * Inject a dummy record from the next epoch. In test 0, this should never
     * get used because the message sequence number is too big. In test 1 we set
     * the record sequence number to be way off in the future. This should not
     * have an impact on the record replay protection because the record should
     * be dropped before it is marked as arrived
     */
    c_to_s_mempacket = SSL_get_wbio(clientssl1);
    c_to_s_mempacket = BIO_next(c_to_s_mempacket);
    mempacket_test_inject(c_to_s_mempacket, (char *)certstatus,
                          sizeof(certstatus), 1, INJECT_PACKET_IGNORE_REC_SEQ);

    if (!create_ssl_connection(serverssl1, clientssl1)) {
        printf("Unable to create SSL connection\n");
        ERR_print_errors_fp(stdout);
        goto end;
    }

    testresult = 1;
 end:
    SSL_free(serverssl1);
    SSL_free(clientssl1);
    SSL_CTX_free(sctx);
    SSL_CTX_free(cctx);

    return testresult;
}

int main(int argc, char *argv[])
{
    BIO *err = NULL;
    int testresult = 1;

    if (argc != 3) {
        printf("Invalid argument count\n");
        return 1;
    }

    cert = argv[1];
    privkey = argv[2];

    err = BIO_new_fp(stderr, BIO_NOCLOSE | BIO_FP_TEXT);

    CRYPTO_set_mem_debug(1);
    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

    ADD_ALL_TESTS(test_dtls_unprocessed, NUM_TESTS);

    testresult = run_tests(argv[0]);

    bio_f_tls_dump_filter_free();
    bio_s_mempacket_test_free();

#ifndef OPENSSL_NO_CRYPTO_MDEBUG
    if (CRYPTO_mem_leaks(err) <= 0)
        testresult = 1;
#endif
    BIO_free(err);

    if (!testresult)
        printf("PASS\n");

    return testresult;
}
