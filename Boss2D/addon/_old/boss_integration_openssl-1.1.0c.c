﻿#include <boss.h>
#if BOSS_NEED_ADDON_CURL | BOSS_NEED_ADDON_GIT | BOSS_NEED_ADDON_SSL | BOSS_NEED_ADDON_WEBRTC | BOSS_NEED_ADDON_WEBSOCKET

#include <addon/boss_integration_openssl-1.1.0c.h>

#if BOSS_WINDOWS
    #pragma comment(lib, "advapi32.lib")
    #pragma comment(lib, "gdi32.lib")
#endif

#pragma warning(disable : 4090)
#pragma warning(disable : 4133)

#if !BOSS_WINDOWS | BOSS_NEED_FORCED_FAKEWIN
    #define __pragma(...)
    #define struct_stat_BOSS struct boss_fakewin_struct_stat64
#else
    #define struct_stat_BOSS struct stat
#endif

#include <addon/openssl-1.1.0c_for_boss/crypto/ex_data.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/mem.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/mem_clr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cryptlib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_dir.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_init.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_time.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/mem_dbg.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cversion.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/threads_none.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_str.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_fips.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/init.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/mem_sec.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/o_fopen.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cpt_err.c>

#include <addon/openssl-1.1.0c_for_boss/crypto/objects/obj_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/objects/obj_dat.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/objects/obj_xref.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/objects/o_names.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/objects/obj_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/err/err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/err/err_prn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/err/err_all.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bio_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_bio.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_file.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_null.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_mem.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/b_print.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/b_dump.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_sock.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/b_sock.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_conn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bf_buff.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bss_dgram.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/b_addr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/b_sock2.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bio_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bio/bio_meth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_gen.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_crpt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_sign.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_x931.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_none.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_ssl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_oaep.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_pk1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_pss.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_saos.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_ameth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_ossl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rsa/rsa_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_gen.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_ossl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_sign.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_vrf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_ameth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dsa/dsa_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rc4/rc4_skey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rc4/rc4_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/sha/sha1dgst.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/sha/sha256.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/sha/sha512.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/sha/sha1_one.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/md5/md5_dgst.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/md5/md5_one.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_mult.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_oct.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_curve.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_cvt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_ameth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_print.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecp_oct.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecp_nist.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecp_smpl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecp_mont.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec2_oct.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec2_smpl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec2_mult.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/eck_prn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecdh_ossl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_kmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecx_meth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecdh_kdf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecdsa_sign.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecdsa_ossl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ecdsa_vrf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/curve25519.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ec/ec_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_cmp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_req.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_att.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_v3.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_ext.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_set.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_obj.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_def.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_vfy.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_trs.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_vpm.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_lu.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_txt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_d2.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509cset.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509rset.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509name.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_x509a.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_all.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_crl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_x509.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_req.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_exten.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_name.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_attrib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/t_x509.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/by_file.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/by_dir.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x_pubkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509/x509type.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_cache.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_map.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_data.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_tree.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/pcy_node.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_info.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_genn.c>
#define extract_min_max extract_min_max_v3_addr_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_addr.c>
#undef extract_min_max
#define extract_min_max extract_min_max_v3_asid_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_asid.c>
#undef extract_min_max
#define str_free str_free_v3_utl_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_utl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_akey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_akeya.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_alt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_bcons.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_cpols.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_purp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_conf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_pcons.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_ncons.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_pmaps.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_crld.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_pcia.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_prn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_int.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_pci.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_extku.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_enum.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_skey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_ia5.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_sxnet.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_pku.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_bitst.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3_tlsf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/x509v3/v3err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs7/pk7_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs7/pk7_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs7/pk7_doit.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs7/pk7_attr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs7/pkcs7err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_p8d.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_decr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_p8e.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_crpt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_utl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_asn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/p12_add.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pkcs12/pk12err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_ht.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_asn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/v3_ocsp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_cl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_prn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_vfy.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ocsp/ocsp_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_pkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_pbe.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/digest.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/names.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/encode.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/scrypt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/c_allc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/c_alld.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/p_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/p_verify.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/p_sign.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/p5_crpt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/p5_crpt2.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/pmeth_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/pmeth_fn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/pmeth_gn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_sha1.c>
#define init init_m_md5_BOSS
#define update update_m_md5_BOSS
#define final final_m_md5_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_md5.c>
#undef init
#undef update
#undef final
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_sigver.c>
#define init init_m_ripemd_BOSS
#define update update_m_ripemd_BOSS
#define final final_m_ripemd_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_ripemd.c>
#undef init
#undef update
#undef final
#define init init_m_wp_BOSS
#define update update_m_wp_BOSS
#define final final_m_wp_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_wp.c>
#undef init
#undef update
#undef final
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/bio_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/bio_md.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_rc4.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_rc4_hmac_md5.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_aes.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_aes_cbc_hmac_sha1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_aes_cbc_hmac_sha256.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_old.c>
#define init init_m_md5_sha1_BOSS
#define update update_m_md5_sha1_BOSS
#define final final_m_md5_sha1_BOSS
#define ctrl ctrl_m_md5_sha1_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/m_md5_sha1.c>
#undef init
#undef update
#undef final
#undef ctrl
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_null.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_xcbc_d.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_chacha20_poly1305.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_cnf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/evp_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_des.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/evp/e_des3.c>
#undef ks1
#undef ks2
#undef ks3
#include <addon/openssl-1.1.0c_for_boss/crypto/ui/ui_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ui/ui_openssl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ui/ui_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn1_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn1_gen.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn1_par.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_type.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_object.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_d2i_fp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_i2d_fp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_octet.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_time.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_int.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_bitstr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_gentm.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_utctm.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_strex.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_sign.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_verify.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_digest.c>
#define table table_a_strnid_BOSS
#define table_cmp table_a_strnid_BOSS_cmp
#define table_cmp_BSEARCH_CMP_FN table_a_strnid_BOSS_cmp_BSEARCH_CMP_FN
#define OBJ_bsearch_table OBJ_bsearch_table_a_strnid_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_strnid.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_mbstr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_dup.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_print.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/a_utf8.c>
#define standard_methods standard_methods_ameth_lib_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/ameth_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/i2d_pr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/d2i_pr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/p5_pbev2.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/p5_pbe.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/p5_scrypt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/p8_pkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/evp_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn_pack.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_fre.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_dec.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_typ.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_utl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_new.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/tasn_prn.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/t_pkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/f_int.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/f_string.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/nsseq.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_sig.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_bignum.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_long.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_spki.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_algor.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_val.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_info.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn_moid.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/x_pkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn1_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/asn1/asn_mstbl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_ctx.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_add.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_mul.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_div.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_mod.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_gcd.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_sqr.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_word.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_shift.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_mont.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_exp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_blind.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_rand.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_recp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_intern.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_nist.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_gf2m.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_sqrt.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_kron.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_print.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_prime.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_exp2.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_srp.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_const.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_dh.c>
#if BOSS_WINDOWS & !BOSS_WINDOWS_MINGW
    #include <addon/openssl-1.1.0c_for_boss/crypto/bn/asm/x86_64-gcc.c> // assembly code
#else
    #include <addon/openssl-1.1.0c_for_boss/crypto/bn/bn_asm.c>
#endif

#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_all.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_pkey.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_pk8.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_oth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_x509.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_info.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/pem/pem_xaux.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rand/md_rand.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rand/rand_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rand/randfile.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/rand/rand_err.c>
#if BOSS_WINDOWS && !BOSS_NEED_FORCED_FAKEWIN
    #include <addon/openssl-1.1.0c_for_boss/crypto/rand/rand_unix.c>
    #include <addon/openssl-1.1.0c_for_boss/crypto/rand/rand_vms.c>
    #include <addon/openssl-1.1.0c_for_boss/crypto/rand/rand_win.c>
#else
    unsigned int arc4random()
    {
        return rand() & 0xFFFFFFFF;
    }
    int RAND_poll(void)
    {
        unsigned int rnd = 0, i;
        unsigned char buf[ENTROPY_NEEDED];

        for (i = 0; i < sizeof(buf); i++) {
            if (i % 4 == 0)
                rnd = arc4random();
            buf[i] = rnd;
            rnd >>= 8;
        }
        RAND_add(buf, sizeof(buf), ENTROPY_NEEDED);
        memset(buf, 0, sizeof(buf));

        return 1;
    }
#endif
void rc4_md5_enc(RC4_KEY *key, const void *in0, void *out,
    MD5_CTX *ctx, const void *inp, size_t blocks)
{
    BOSS_ASSERT("rc4_md5_enc호출은 추가개발필요", 0);
}

#include <addon/openssl-1.1.0c_for_boss/crypto/stack/stack.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/buffer/buffer.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/buffer/buf_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/lhash/lhash.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_mod.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_api.c>
#define str_copy str_copy_conf_def_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_def.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_mall.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_sap.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/conf/conf_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dso/dso_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dso/dso_openssl.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dso/dso_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cmac/cmac.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cmac/cm_ameth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/cmac/cm_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/hmac/hmac.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/hmac/hm_ameth.c>
#define int_update int_update_hm_pmeth_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/hmac/hm_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ripemd/rmd_dgst.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/whrlpool/wp_dgst.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/whrlpool/wp_block.c>
#undef N
#define ctr64_inc ctr64_inc_ccm128_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/ccm128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/cbc128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/ctr128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/cfb128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/ocb128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/ofb128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/gcm128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/wrap128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/modes/xts128.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/aes/aes_core.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/aes/aes_cbc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/async/async_wait.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/async/async.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/async/async_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/async/arch/async_win.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/srp/srp_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/srp/srp_vfy.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_ameth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_pmeth.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_gen.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_check.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_rfc5114.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/dh/dh_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/comp/comp_lib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/comp/c_zlib.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/comp/comp_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/ecb_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/set_key.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/des_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/xcbc_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/ecb3_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/cfb_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/cfb64enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/cfb64ede.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/ofb64enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/des/ofb64ede.c>
#define init init_m_blake2b_BOSS
#define update update_m_blake2b_BOSS
#define final final_m_blake2b_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/blake2/m_blake2b.c>
#undef init
#undef update
#undef final
#include <addon/openssl-1.1.0c_for_boss/crypto/blake2/blake2b.c>
#define init init_m_blake2s_BOSS
#define update update_m_blake2s_BOSS
#define final final_m_blake2s_BOSS
#include <addon/openssl-1.1.0c_for_boss/crypto/blake2/m_blake2s.c>
#undef init
#undef update
#undef final
#include <addon/openssl-1.1.0c_for_boss/crypto/blake2/blake2s.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/kdf/hkdf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/kdf/tls1_prf.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/kdf/kdf_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/chacha/chacha_enc.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/poly1305/poly1305.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/ts/ts_err.c>
#include <addon/openssl-1.1.0c_for_boss/crypto/txt_db/txt_db.c>

#include <addon/openssl-1.1.0c_for_boss/ssl/bio_ssl.c>
#define get_current_time get_current_time_d1_lib_BOSS
#include <addon/openssl-1.1.0c_for_boss/ssl/d1_lib.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/d1_msg.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/d1_srtp.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/methods.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/pqueue.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/s3_cbc.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/s3_enc.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/s3_lib.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/s3_msg.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_asn1.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_cert.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_ciph.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_conf.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_err.c>
#define stopped stopped_ssl_init_BOSS
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_init.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_lib.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_mcnf.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_rsa.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_sess.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_stat.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_txt.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/ssl_utst.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/t1_enc.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/t1_ext.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/t1_lib.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/t1_reneg.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/t1_trce.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/tls_srp.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/record/dtls1_bitmap.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/record/rec_layer_d1.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/record/rec_layer_s3.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/record/ssl3_buffer.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/record/ssl3_record.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/statem/statem.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/statem/statem_clnt.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/statem/statem_dtls.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/statem/statem_lib.c>
#include <addon/openssl-1.1.0c_for_boss/ssl/statem/statem_srvr.c>

#endif