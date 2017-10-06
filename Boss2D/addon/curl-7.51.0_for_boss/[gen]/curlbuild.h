#ifndef __CURL_CURLBUILD_H
#define __CURL_CURLBUILD_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2012, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

/* ================================================================ */
/*               NOTES FOR CONFIGURE CAPABLE SYSTEMS                */
/* ================================================================ */

/*
 * NOTE 1:
 * -------
 *
 * Nothing in this file is intended to be modified or adjusted by the
 * curl library user nor by the curl library builder.
 *
 * If you think that something actually needs to be changed, adjusted
 * or fixed in this file, then, report it on the libcurl development
 * mailing list: https://cool.haxx.se/mailman/listinfo/curl-library/
 *
 * This header file shall only export symbols which are 'curl' or 'CURL'
 * prefixed, otherwise public name space would be polluted.
 *
 * NOTE 2:
 * -------
 *
 * Right now you might be staring at file include/curl/curlbuild.h.in or
 * at file include/curl/curlbuild.h, this is due to the following reason:
 *
 * On systems capable of running the configure script, the configure process
 * will overwrite the distributed include/curl/curlbuild.h file with one that
 * is suitable and specific to the library being configured and built, which
 * is generated from the include/curl/curlbuild.h.in template file.
 *
 */

/* ================================================================ */
/*  DEFINITION OF THESE SYMBOLS SHALL NOT TAKE PLACE ANYWHERE ELSE  */
/* ================================================================ */

#ifdef CURL_SIZEOF_LONG
#error "CURL_SIZEOF_LONG shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_LONG_already_defined
#endif

#ifdef CURL_TYPEOF_CURL_SOCKLEN_T
#error "CURL_TYPEOF_CURL_SOCKLEN_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_TYPEOF_CURL_SOCKLEN_T_already_defined
#endif

#ifdef CURL_SIZEOF_CURL_SOCKLEN_T
#error "CURL_SIZEOF_CURL_SOCKLEN_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_CURL_SOCKLEN_T_already_defined
#endif

#ifdef CURL_TYPEOF_CURL_OFF_T
#error "CURL_TYPEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_TYPEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_T
#error "CURL_FORMAT_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_TU
#error "CURL_FORMAT_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_TU_already_defined
#endif

#ifdef CURL_FORMAT_OFF_T
#error "CURL_FORMAT_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_OFF_T_already_defined
#endif

#ifdef CURL_SIZEOF_CURL_OFF_T
#error "CURL_SIZEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_T
#error "CURL_SUFFIX_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_TU
#error "CURL_SUFFIX_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_TU_already_defined
#endif

/* ================================================================ */
/*  EXTERNAL INTERFACE SETTINGS FOR CONFIGURE CAPABLE SYSTEMS ONLY  */
/* ================================================================ */

/* Configure process defines this to 1 when it finds out that system  */
/* header file ws2tcpip.h must be included by the external interface. */
//#undef CURL_PULL_WS2TCPIP_H
#ifdef CURL_PULL_WS2TCPIP_H
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include BOSS_FAKEWIN_V_windows_h //original-code:<windows.h>
#  include BOSS_FAKEWIN_V_winsock2_h //original-code:<winsock2.h>
#  include BOSS_FAKEWIN_V_ws2tcpip_h //original-code:<ws2tcpip.h>
#endif

/* Configure process defines this to 1 when it finds out that system   */
/* header file sys/types.h must be included by the external interface. */
//#undef CURL_PULL_SYS_TYPES_H
#ifdef CURL_PULL_SYS_TYPES_H
#  include <sys/types.h>
#endif

/* Configure process defines this to 1 when it finds out that system */
/* header file stdint.h must be included by the external interface.  */
//#undef CURL_PULL_STDINT_H
#ifdef CURL_PULL_STDINT_H
#  include BOSS_FAKEWIN_V_stdint_h //original-code:<stdint.h>
#endif

/* Configure process defines this to 1 when it finds out that system  */
/* header file inttypes.h must be included by the external interface. */
//#undef CURL_PULL_INTTYPES_H
#ifdef CURL_PULL_INTTYPES_H
#  include <inttypes.h>
#endif

/* Configure process defines this to 1 when it finds out that system    */
/* header file sys/socket.h must be included by the external interface. */
//#undef CURL_PULL_SYS_SOCKET_H
#ifdef CURL_PULL_SYS_SOCKET_H
#  include <sys/socket.h>
#endif

/* Configure process defines this to 1 when it finds out that system  */
/* header file sys/poll.h must be included by the external interface. */
//#undef CURL_PULL_SYS_POLL_H
#ifdef CURL_PULL_SYS_POLL_H
#  include <sys/poll.h>
#endif

#if BOSS_MAC_OSX || BOSS_IPHONE
    #define CURL_SIZEOF_LONG           8
    #define CURL_TYPEOF_CURL_SOCKLEN_T int
    #define CURL_SIZEOF_CURL_SOCKLEN_T 4
    #define CURL_TYPEOF_CURL_OFF_T     long long
    #define CURL_FORMAT_CURL_OFF_T     "lld"
    #define CURL_FORMAT_CURL_OFF_TU    "llu"
    #define CURL_FORMAT_OFF_T          "%lld"
    #define CURL_SIZEOF_CURL_OFF_T     8
    #define CURL_SUFFIX_CURL_OFF_T     LL
    #define CURL_SUFFIX_CURL_OFF_TU    ULL
#else
    #define CURL_SIZEOF_LONG           4
    #define CURL_TYPEOF_CURL_SOCKLEN_T int
    #define CURL_SIZEOF_CURL_SOCKLEN_T 4
    #define CURL_TYPEOF_CURL_OFF_T     long long
    #define CURL_FORMAT_CURL_OFF_T     "lld"
    #define CURL_FORMAT_CURL_OFF_TU    "llu"
    #define CURL_FORMAT_OFF_T          "%lld"
    #define CURL_SIZEOF_CURL_OFF_T     8
    #define CURL_SUFFIX_CURL_OFF_T     LL
    #define CURL_SUFFIX_CURL_OFF_TU    ULL
#endif

/* The size of `long', as computed by sizeof. */
//#undef CURL_SIZEOF_LONG

/* Integral data type used for curl_socklen_t. */
//#undef CURL_TYPEOF_CURL_SOCKLEN_T

/* The size of `curl_socklen_t', as computed by sizeof. */
//#undef CURL_SIZEOF_CURL_SOCKLEN_T

/* Data type definition of curl_socklen_t. */
typedef CURL_TYPEOF_CURL_SOCKLEN_T curl_socklen_t;

/* Signed integral data type used for curl_off_t. */
//#undef CURL_TYPEOF_CURL_OFF_T

/* Data type definition of curl_off_t. */
typedef CURL_TYPEOF_CURL_OFF_T curl_off_t;

/* curl_off_t formatting string directive without "%" conversion specifier. */
//#undef CURL_FORMAT_CURL_OFF_T

/* unsigned curl_off_t formatting string without "%" conversion specifier. */
//#undef CURL_FORMAT_CURL_OFF_TU

/* curl_off_t formatting string directive with "%" conversion specifier. */
//#undef CURL_FORMAT_OFF_T

/* The size of `curl_off_t', as computed by sizeof. */
//#undef CURL_SIZEOF_CURL_OFF_T

/* curl_off_t constant suffix. */
//#undef CURL_SUFFIX_CURL_OFF_T

/* unsigned curl_off_t constant suffix. */
//#undef CURL_SUFFIX_CURL_OFF_TU

#endif /* __CURL_CURLBUILD_H */
