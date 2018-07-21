﻿#pragma once
#include <addon/boss_fakewin.h>
#include <addon/boss_integration_opencv-3.1.0.h>
#include <addon/boss_integration_openalpr-2.3.0_3rdparty_liblept.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// _BOSS_BATCH_COMMAND_ {"type" : "replace", "prm" : "BOSS_TESSERACT_", "restore-comment" : " //original-code:"}

#define BOSS_TESSERACT_wfopen                       boss_fakewin_wfopen
#define BOSS_TESSERACT__wfopen                      boss_fakewin_wfopen
#define BOSS_TESSERACT_wfopen_s                     boss_fakewin_wfopen_s
#define BOSS_TESSERACT__wfopen_s                    boss_fakewin_wfopen_s
#define BOSS_TESSERACT_fopen                        boss_fakewin_fopen
#define BOSS_TESSERACT_fopen_s                      boss_fakewin_fopen_s
#define BOSS_TESSERACT_fseek                        boss_fakewin_fseek
#define BOSS_TESSERACT_ftell                        boss_fakewin_ftell
#define BOSS_TESSERACT_rewind                       boss_fakewin_rewind
#define BOSS_TESSERACT_fread                        boss_fakewin_fread
#define BOSS_TESSERACT_fwrite                       boss_fakewin_fwrite
#define BOSS_TESSERACT_fgetc                        boss_fakewin_fgetc
#define BOSS_TESSERACT_fgets                        boss_fakewin_fgets
#define BOSS_TESSERACT_ungetc                       boss_fakewin_ungetc
#define BOSS_TESSERACT_fclose                       boss_fakewin_fclose

////////////////////////////////////////////////////////////////////////////////////////////////////
// _BOSS_BATCH_COMMAND_ {"type" : "include-alias", "prm" : "BOSS_TESSERACT_", "restore-comment" : " //original-code:"}
#define BOSS_TESSERACT_V_mathfix_h                  <addon/tesseract-3.05.02_for_boss/vs2010/port/mathfix.h>

#define BOSS_TESSERACT_U_mathfix_h                  "addon/tesseract-3.05.02_for_boss/vs2010/port/mathfix.h"
#define BOSS_TESSERACT_U_vcsversion_h               "addon/tesseract-3.05.02_for_boss/vs2010/port/vcsversion.h"

#define BOSS_TESSERACT_U_baseapi_h                  "addon/tesseract-3.05.02_for_boss/api/baseapi.h"
#define BOSS_TESSERACT_U_control_h                  "addon/tesseract-3.05.02_for_boss/ccmain/control.h"
#define BOSS_TESSERACT_U_resultiterator_h           "addon/tesseract-3.05.02_for_boss/ccmain/resultiterator.h"
#define BOSS_TESSERACT_U_mutableiterator_h          "addon/tesseract-3.05.02_for_boss/ccmain/mutableiterator.h"
#define BOSS_TESSERACT_U_thresholder_h              "addon/tesseract-3.05.02_for_boss/ccmain/thresholder.h"
#define BOSS_TESSERACT_U_tesseractclass_h           "addon/tesseract-3.05.02_for_boss/ccmain/tesseractclass.h"
#define BOSS_TESSERACT_U_paragraphs_h               "addon/tesseract-3.05.02_for_boss/ccmain/paragraphs.h"
#define BOSS_TESSERACT_U_tessvars_h                 "addon/tesseract-3.05.02_for_boss/ccmain/tessvars.h"
#define BOSS_TESSERACT_U_pgedit_h                   "addon/tesseract-3.05.02_for_boss/ccmain/pgedit.h"
#define BOSS_TESSERACT_U_paramsd_h                  "addon/tesseract-3.05.02_for_boss/ccmain/paramsd.h"
#define BOSS_TESSERACT_U_output_h                   "addon/tesseract-3.05.02_for_boss/ccmain/output.h"
#define BOSS_TESSERACT_U_equationdetect_h           "addon/tesseract-3.05.02_for_boss/ccmain/equationdetect.h"
#define BOSS_TESSERACT_U_tessbox_h                  "addon/tesseract-3.05.02_for_boss/ccmain/tessbox.h"
#define BOSS_TESSERACT_U_osdetect_h                 "addon/tesseract-3.05.02_for_boss/ccmain/osdetect.h"
#define BOSS_TESSERACT_U_pageiterator_h             "addon/tesseract-3.05.02_for_boss/ccmain/pageiterator.h"
#define BOSS_TESSERACT_U_cube_reco_context_h        "addon/tesseract-3.05.02_for_boss/ccmain/cube_reco_context.h"
#define BOSS_TESSERACT_U_globaloc_h                 "addon/tesseract-3.05.02_for_boss/ccutil/globaloc.h"
#define BOSS_TESSERACT_U_params_h                   "addon/tesseract-3.05.02_for_boss/ccutil/params.h"
#define BOSS_TESSERACT_U_strngs_h                   "addon/tesseract-3.05.02_for_boss/ccutil/strngs.h"
#define BOSS_TESSERACT_U_platform_h                 "addon/tesseract-3.05.02_for_boss/ccutil/platform.h"
#define BOSS_TESSERACT_U_unichar_h                  "addon/tesseract-3.05.02_for_boss/ccutil/unichar.h"
#define BOSS_TESSERACT_U_tesscallback_h             "addon/tesseract-3.05.02_for_boss/ccutil/tesscallback.h"
#define BOSS_TESSERACT_U_elst_h                     "addon/tesseract-3.05.02_for_boss/ccutil/elst.h"
#define BOSS_TESSERACT_U_serialis_h                 "addon/tesseract-3.05.02_for_boss/ccutil/serialis.h"
#define BOSS_TESSERACT_U_memry_h                    "addon/tesseract-3.05.02_for_boss/ccutil/memry.h"
#define BOSS_TESSERACT_U_ndminx_h                   "addon/tesseract-3.05.02_for_boss/ccutil/ndminx.h"
#define BOSS_TESSERACT_U_tprintf_h                  "addon/tesseract-3.05.02_for_boss/ccutil/tprintf.h"
#define BOSS_TESSERACT_U_bits16_h                   "addon/tesseract-3.05.02_for_boss/ccutil/bits16.h"
#define BOSS_TESSERACT_U_elst2_h                    "addon/tesseract-3.05.02_for_boss/ccutil/elst2.h"
#define BOSS_TESSERACT_U_errcode_h                  "addon/tesseract-3.05.02_for_boss/ccutil/errcode.h"
#define BOSS_TESSERACT_U_clst_h                     "addon/tesseract-3.05.02_for_boss/ccutil/clst.h"
#define BOSS_TESSERACT_U_tessdatamanager_h          "addon/tesseract-3.05.02_for_boss/ccutil/tessdatamanager.h"
#define BOSS_TESSERACT_U_genericvector_h            "addon/tesseract-3.05.02_for_boss/ccutil/genericvector.h"
#define BOSS_TESSERACT_U_host_h                     "addon/tesseract-3.05.02_for_boss/ccutil/host.h"
#define BOSS_TESSERACT_U_kdpair_h                   "addon/tesseract-3.05.02_for_boss/ccutil/kdpair.h"
#define BOSS_TESSERACT_U_unicharset_h               "addon/tesseract-3.05.02_for_boss/ccutil/unicharset.h"
#define BOSS_TESSERACT_U_ccutil_h                   "addon/tesseract-3.05.02_for_boss/ccutil/ccutil.h"
#define BOSS_TESSERACT_U_hashfn_h                   "addon/tesseract-3.05.02_for_boss/ccutil/hashfn.h"
#define BOSS_TESSERACT_U_ambigs_h                   "addon/tesseract-3.05.02_for_boss/ccutil/ambigs.h"
#define BOSS_TESSERACT_U_unicity_table_h            "addon/tesseract-3.05.02_for_boss/ccutil/unicity_table.h"
#define BOSS_TESSERACT_U_genericheap_h              "addon/tesseract-3.05.02_for_boss/ccutil/genericheap.h"
#define BOSS_TESSERACT_U_object_cache_h             "addon/tesseract-3.05.02_for_boss/ccutil/object_cache.h"
#define BOSS_TESSERACT_U_helpers_h                  "addon/tesseract-3.05.02_for_boss/ccutil/helpers.h"
#define BOSS_TESSERACT_U_unicodes_h                 "addon/tesseract-3.05.02_for_boss/ccutil/unicodes.h"
#define BOSS_TESSERACT_U_ocrclass_h                 "addon/tesseract-3.05.02_for_boss/ccutil/ocrclass.h"
#define BOSS_TESSERACT_U_basedir_h                  "addon/tesseract-3.05.02_for_boss/ccutil/basedir.h"
#define BOSS_TESSERACT_U_nwmain_h                   "addon/tesseract-3.05.02_for_boss/ccutil/nwmain.h"
#define BOSS_TESSERACT_U_qrsequence_h               "addon/tesseract-3.05.02_for_boss/ccutil/qrsequence.h"
#define BOSS_TESSERACT_U_scanutils_h                "addon/tesseract-3.05.02_for_boss/ccutil/scanutils.h"
#define BOSS_TESSERACT_U_sorthelper_h               "addon/tesseract-3.05.02_for_boss/ccutil/sorthelper.h"
#define BOSS_TESSERACT_U_stderr_h                   "addon/tesseract-3.05.02_for_boss/ccutil/stderr.h"
#define BOSS_TESSERACT_U_bitvector_h                "addon/tesseract-3.05.02_for_boss/ccutil/bitvector.h"
#define BOSS_TESSERACT_U_fileerr_h                  "addon/tesseract-3.05.02_for_boss/ccutil/fileerr.h"
#define BOSS_TESSERACT_U_indexmapbidi_h             "addon/tesseract-3.05.02_for_boss/ccutil/indexmapbidi.h"
#define BOSS_TESSERACT_U_cutil_h                    "addon/tesseract-3.05.02_for_boss/cutil/cutil.h"
#define BOSS_TESSERACT_U_globals_h                  "addon/tesseract-3.05.02_for_boss/cutil/globals.h"
#define BOSS_TESSERACT_U_oldlist_h                  "addon/tesseract-3.05.02_for_boss/cutil/oldlist.h"
#define BOSS_TESSERACT_U_bitvec_h                   "addon/tesseract-3.05.02_for_boss/cutil/bitvec.h"
#define BOSS_TESSERACT_U_callcpp_h                  "addon/tesseract-3.05.02_for_boss/cutil/callcpp.h"
#define BOSS_TESSERACT_U_const_h                    "addon/tesseract-3.05.02_for_boss/cutil/const.h"
#define BOSS_TESSERACT_U_danerror_h                 "addon/tesseract-3.05.02_for_boss/cutil/danerror.h"
#define BOSS_TESSERACT_U_efio_h                     "addon/tesseract-3.05.02_for_boss/cutil/efio.h"
#define BOSS_TESSERACT_U_emalloc_h                  "addon/tesseract-3.05.02_for_boss/cutil/emalloc.h"
#define BOSS_TESSERACT_U_structures_h               "addon/tesseract-3.05.02_for_boss/cutil/structures.h"
#define BOSS_TESSERACT_U_freelist_h                 "addon/tesseract-3.05.02_for_boss/cutil/freelist.h"
#define BOSS_TESSERACT_U_pageres_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/pageres.h"
#define BOSS_TESSERACT_U_ccstruct_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/ccstruct.h"
#define BOSS_TESSERACT_U_otsuthr_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/otsuthr.h"
#define BOSS_TESSERACT_U_publictypes_h              "addon/tesseract-3.05.02_for_boss/ccstruct/publictypes.h"
#define BOSS_TESSERACT_U_ocrblock_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/ocrblock.h"
#define BOSS_TESSERACT_U_ratngs_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/ratngs.h"
#define BOSS_TESSERACT_U_statistc_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/statistc.h"
#define BOSS_TESSERACT_U_coutln_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/coutln.h"
#define BOSS_TESSERACT_U_rect_h                     "addon/tesseract-3.05.02_for_boss/ccstruct/rect.h"
#define BOSS_TESSERACT_U_blobbox_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/blobbox.h"
#define BOSS_TESSERACT_U_blobs_h                    "addon/tesseract-3.05.02_for_boss/ccstruct/blobs.h"
#define BOSS_TESSERACT_U_seam_h                     "addon/tesseract-3.05.02_for_boss/ccstruct/seam.h"
#define BOSS_TESSERACT_U_split_h                    "addon/tesseract-3.05.02_for_boss/ccstruct/split.h"
#define BOSS_TESSERACT_U_werd_h                     "addon/tesseract-3.05.02_for_boss/ccstruct/werd.h"
#define BOSS_TESSERACT_U_crakedge_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/crakedge.h"
#define BOSS_TESSERACT_U_fontinfo_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/fontinfo.h"
#define BOSS_TESSERACT_U_imagedata_h                "addon/tesseract-3.05.02_for_boss/ccstruct/imagedata.h"
#define BOSS_TESSERACT_U_matrix_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/matrix.h"
#define BOSS_TESSERACT_U_normalis_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/normalis.h"
#define BOSS_TESSERACT_U_ocrpara_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/ocrpara.h"
#define BOSS_TESSERACT_U_ocrrow_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/ocrrow.h"
#define BOSS_TESSERACT_U_params_training_featdef_h  "addon/tesseract-3.05.02_for_boss/ccstruct/params_training_featdef.h"
#define BOSS_TESSERACT_U_boxread_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/boxread.h"
#define BOSS_TESSERACT_U_blamer_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/blamer.h"
#define BOSS_TESSERACT_U_blread_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/blread.h"
#define BOSS_TESSERACT_U_genblob_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/genblob.h"
#define BOSS_TESSERACT_U_quadlsq_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/quadlsq.h"
#define BOSS_TESSERACT_U_detlinefit_h               "addon/tesseract-3.05.02_for_boss/ccstruct/detlinefit.h"
#define BOSS_TESSERACT_U_dppoint_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/dppoint.h"
#define BOSS_TESSERACT_U_linlsq_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/linlsq.h"
#define BOSS_TESSERACT_U_pdblock_h                  "addon/tesseract-3.05.02_for_boss/ccstruct/pdblock.h"
#define BOSS_TESSERACT_U_points_h                   "addon/tesseract-3.05.02_for_boss/ccstruct/points.h"
#define BOSS_TESSERACT_U_vecfuncs_h                 "addon/tesseract-3.05.02_for_boss/ccstruct/vecfuncs.h"
#define BOSS_TESSERACT_U_edgblob_h                  "addon/tesseract-3.05.02_for_boss/textord/edgblob.h"
#define BOSS_TESSERACT_U_makerow_h                  "addon/tesseract-3.05.02_for_boss/textord/makerow.h"
#define BOSS_TESSERACT_U_devanagari_processing_h    "addon/tesseract-3.05.02_for_boss/textord/devanagari_processing.h"
#define BOSS_TESSERACT_U_textord_h                  "addon/tesseract-3.05.02_for_boss/textord/textord.h"
#define BOSS_TESSERACT_U_equationdetectbase_h       "addon/tesseract-3.05.02_for_boss/textord/equationdetectbase.h"
#define BOSS_TESSERACT_U_bbgrid_h                   "addon/tesseract-3.05.02_for_boss/textord/bbgrid.h"
#define BOSS_TESSERACT_U_colpartition_h             "addon/tesseract-3.05.02_for_boss/textord/colpartition.h"
#define BOSS_TESSERACT_U_colpartitiongrid_h         "addon/tesseract-3.05.02_for_boss/textord/colpartitiongrid.h"
#define BOSS_TESSERACT_U_colpartitionset_h          "addon/tesseract-3.05.02_for_boss/textord/colpartitionset.h"
#define BOSS_TESSERACT_U_colfind_h                  "addon/tesseract-3.05.02_for_boss/textord/colfind.h"
#define BOSS_TESSERACT_U_imagefind_h                "addon/tesseract-3.05.02_for_boss/textord/imagefind.h"
#define BOSS_TESSERACT_U_linefind_h                 "addon/tesseract-3.05.02_for_boss/textord/linefind.h"
#define BOSS_TESSERACT_U_tabvector_h                "addon/tesseract-3.05.02_for_boss/textord/tabvector.h"
#define BOSS_TESSERACT_U_tordmain_h                 "addon/tesseract-3.05.02_for_boss/textord/tordmain.h"
#define BOSS_TESSERACT_U_wordseg_h                  "addon/tesseract-3.05.02_for_boss/textord/wordseg.h"
#define BOSS_TESSERACT_U_dict_h                     "addon/tesseract-3.05.02_for_boss/dict/dict.h"
#define BOSS_TESSERACT_U_matchdefs_h                "addon/tesseract-3.05.02_for_boss/dict/matchdefs.h"
#define BOSS_TESSERACT_U_dawg_h                     "addon/tesseract-3.05.02_for_boss/dict/dawg.h"
#define BOSS_TESSERACT_U_stopper_h                  "addon/tesseract-3.05.02_for_boss/dict/stopper.h"
#define BOSS_TESSERACT_U_trie_h                     "addon/tesseract-3.05.02_for_boss/dict/trie.h"
#define BOSS_TESSERACT_U_blobclass_h                "addon/tesseract-3.05.02_for_boss/classify/blobclass.h"
#define BOSS_TESSERACT_U_classify_h                 "addon/tesseract-3.05.02_for_boss/classify/classify.h"
#define BOSS_TESSERACT_U_intproto_h                 "addon/tesseract-3.05.02_for_boss/classify/intproto.h"
#define BOSS_TESSERACT_U_float2int_h                "addon/tesseract-3.05.02_for_boss/classify/float2int.h"
#define BOSS_TESSERACT_U_intmatcher_h               "addon/tesseract-3.05.02_for_boss/classify/intmatcher.h"
#define BOSS_TESSERACT_U_mfoutline_h                "addon/tesseract-3.05.02_for_boss/classify/mfoutline.h"
#define BOSS_TESSERACT_U_shapeclassifier_h          "addon/tesseract-3.05.02_for_boss/classify/shapeclassifier.h"
#define BOSS_TESSERACT_U_tessclassifier_h           "addon/tesseract-3.05.02_for_boss/classify/tessclassifier.h"
#define BOSS_TESSERACT_U_trainingsample_h           "addon/tesseract-3.05.02_for_boss/classify/trainingsample.h"
#define BOSS_TESSERACT_U_featdefs_h                 "addon/tesseract-3.05.02_for_boss/classify/featdefs.h"
#define BOSS_TESSERACT_U_cluster_h                  "addon/tesseract-3.05.02_for_boss/classify/cluster.h"
#define BOSS_TESSERACT_U_clusttool_h                "addon/tesseract-3.05.02_for_boss/classify/clusttool.h"
#define BOSS_TESSERACT_U_intfeaturespace_h          "addon/tesseract-3.05.02_for_boss/classify/intfeaturespace.h"
#define BOSS_TESSERACT_U_mastertrainer_h            "addon/tesseract-3.05.02_for_boss/classify/mastertrainer.h"
#define BOSS_TESSERACT_U_mf_h                       "addon/tesseract-3.05.02_for_boss/classify/mf.h"
#define BOSS_TESSERACT_U_shapetable_h               "addon/tesseract-3.05.02_for_boss/classify/shapetable.h"
#define BOSS_TESSERACT_U_protos_h                   "addon/tesseract-3.05.02_for_boss/classify/protos.h"
#define BOSS_TESSERACT_U_ocrfeatures_h              "addon/tesseract-3.05.02_for_boss/classify/ocrfeatures.h"
#define BOSS_TESSERACT_U_picofeat_h                 "addon/tesseract-3.05.02_for_boss/classify/picofeat.h"
#define BOSS_TESSERACT_U_word_unigrams_h            "addon/tesseract-3.05.02_for_boss/cube/word_unigrams.h"
#define BOSS_TESSERACT_U_char_bigrams_h             "addon/tesseract-3.05.02_for_boss/cube/char_bigrams.h"
#define BOSS_TESSERACT_U_char_set_h                 "addon/tesseract-3.05.02_for_boss/cube/char_set.h"
#define BOSS_TESSERACT_U_classifier_base_h          "addon/tesseract-3.05.02_for_boss/cube/classifier_base.h"
#define BOSS_TESSERACT_U_feature_base_h             "addon/tesseract-3.05.02_for_boss/cube/feature_base.h"
#define BOSS_TESSERACT_U_lang_model_h               "addon/tesseract-3.05.02_for_boss/cube/lang_model.h"
#define BOSS_TESSERACT_U_word_size_model_h          "addon/tesseract-3.05.02_for_boss/cube/word_size_model.h"
#define BOSS_TESSERACT_U_char_altlist_h             "addon/tesseract-3.05.02_for_boss/cube/char_altlist.h"
#define BOSS_TESSERACT_U_classifier_factory_h       "addon/tesseract-3.05.02_for_boss/cube/classifier_factory.h"
#define BOSS_TESSERACT_U_cube_object_h              "addon/tesseract-3.05.02_for_boss/cube/cube_object.h"
#define BOSS_TESSERACT_U_cube_tuning_params_h       "addon/tesseract-3.05.02_for_boss/cube/cube_tuning_params.h"
#define BOSS_TESSERACT_U_cube_utils_h               "addon/tesseract-3.05.02_for_boss/cube/cube_utils.h"
#define BOSS_TESSERACT_U_feature_bmp_h              "addon/tesseract-3.05.02_for_boss/cube/feature_bmp.h"
#define BOSS_TESSERACT_U_tess_lang_model_h          "addon/tesseract-3.05.02_for_boss/cube/tess_lang_model.h"
#define BOSS_TESSERACT_U_word_altlist_h             "addon/tesseract-3.05.02_for_boss/cube/word_altlist.h"
#define BOSS_TESSERACT_U_wordrec_h                  "addon/tesseract-3.05.02_for_boss/wordrec/wordrec.h"
#define BOSS_TESSERACT_U_chopper_h                  "addon/tesseract-3.05.02_for_boss/wordrec/chopper.h"
#define BOSS_TESSERACT_U_chop_h                     "addon/tesseract-3.05.02_for_boss/wordrec/chop.h"
#define BOSS_TESSERACT_U_drawfx_h                   "addon/tesseract-3.05.02_for_boss/wordrec/drawfx.h"
#define BOSS_TESSERACT_U_openclwrapper_h            "addon/tesseract-3.05.02_for_boss/opencl/openclwrapper.h"
#define BOSS_TESSERACT_U_svutil_h                   "addon/tesseract-3.05.02_for_boss/viewer/svutil.h"
#define BOSS_TESSERACT_U_scrollview_h               "addon/tesseract-3.05.02_for_boss/viewer/scrollview.h"
#define BOSS_TESSERACT_U_svmnode_h                  "addon/tesseract-3.05.02_for_boss/viewer/svmnode.h"
#define BOSS_TESSERACT_U_neural_net_h               "addon/tesseract-3.05.02_for_boss/neural_networks/runtime/neural_net.h"
#define BOSS_TESSERACT_U_gettimeofday_h             "addon/tesseract-3.05.02_for_boss/vs2010/port/gettimeofday.h"
#define BOSS_TESSERACT_U_strtok_r_h                 "addon/tesseract-3.05.02_for_boss/vs2010/port/strtok_r.h"
