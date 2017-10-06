﻿#pragma once
#include <addon/boss_fakewin.h>
#include <addon/boss_integration_opencv-3.1.0.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// _BOSS_BATCH_COMMAND_ {"type" : "include-alias", "prm" : "BOSS_OPENALPR_", "restore-comment" : " //original-code:"}
#define BOSS_OPENALPR_V_support__tinythread_h         <addon/openalpr-2.3.0_for_boss/src/openalpr/support/tinythread.h>

#define BOSS_OPENALPR_U_prewarp_h                                "addon/openalpr-2.3.0_for_boss/src/openalpr/prewarp.h"
#define BOSS_OPENALPR_U_utility_h                                "addon/openalpr-2.3.0_for_boss/src/openalpr/utility.h"
#define BOSS_OPENALPR_U_config_h                                 "addon/openalpr-2.3.0_for_boss/src/openalpr/config.h"
#define BOSS_OPENALPR_U_pipeline_data_h                          "addon/openalpr-2.3.0_for_boss/src/openalpr/pipeline_data.h"
#define BOSS_OPENALPR_U_constants_h                              "addon/openalpr-2.3.0_for_boss/src/openalpr/constants.h"
#define BOSS_OPENALPR_U_binarize_wolf_h                          "addon/openalpr-2.3.0_for_boss/src/openalpr/binarize_wolf.h"
#define BOSS_OPENALPR_U_transformation_h                         "addon/openalpr-2.3.0_for_boss/src/openalpr/transformation.h"
#define BOSS_OPENALPR_U_support__filesystem_h                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/filesystem.h"
#define BOSS_OPENALPR_U_support__version_h                       "addon/openalpr-2.3.0_for_boss/src/openalpr/support/version.h"
#define BOSS_OPENALPR_U_support__timing_h                        "addon/openalpr-2.3.0_for_boss/src/openalpr/support/timing.h"
#define BOSS_OPENALPR_U_support__re2_h                           "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2.h"
#define BOSS_OPENALPR_U_support__utf8_h                          "addon/openalpr-2.3.0_for_boss/src/openalpr/support/utf8.h"
#define BOSS_OPENALPR_U_support__tinythread_h                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/tinythread.h"
#define BOSS_OPENALPR_U_textdetection__textline_h                "addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/textline.h"
#define BOSS_OPENALPR_U_textdetection__textcontours_h            "addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/textcontours.h"
#define BOSS_OPENALPR_U_postprocess__postprocess_h               "addon/openalpr-2.3.0_for_boss/src/openalpr/postprocess/postprocess.h"

#define BOSS_OPENALPR_U_re2_h                                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2.h"
#define BOSS_OPENALPR_U_re2__set_h                               "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/set.h"
#define BOSS_OPENALPR_U_re2__prog_h                              "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prog.h"
#define BOSS_OPENALPR_U_re2__regexp_h                            "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/regexp.h"
#define BOSS_OPENALPR_U_re2__stringpiece_h                       "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/stringpiece.h"
#define BOSS_OPENALPR_U_re2__walker_M_inl_h                      "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/walker-inl.h"
#define BOSS_OPENALPR_U_re2__filtered_re2_h                      "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/filtered_re2.h"
#define BOSS_OPENALPR_U_re2__prefilter_h                         "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prefilter.h"
#define BOSS_OPENALPR_U_re2__prefilter_tree_h                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prefilter_tree.h"
#define BOSS_OPENALPR_U_re2__unicode_casefold_h                  "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/unicode_casefold.h"
#define BOSS_OPENALPR_U_re2__unicode_groups_h                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/unicode_groups.h"
#define BOSS_OPENALPR_U_re2__util__logging_h                     "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/logging.h"
#define BOSS_OPENALPR_U_re2__util__mutex_h                       "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/mutex.h"
#define BOSS_OPENALPR_U_re2__util__utf_h                         "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/utf.h"
#define BOSS_OPENALPR_U_re2__util__util_h                        "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/util.h"
#define BOSS_OPENALPR_U_re2__util__valgrind_h                    "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/valgrind.h"
#define BOSS_OPENALPR_U_re2__util__sparse_array_h                "addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/sparse_array.h"
#define BOSS_OPENALPR_U_ocr__segmentation__histogramhorizontal_h "addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/segmentation/histogramhorizontal.h"

// liblept for tesseract
#define BOSS_OPENALPR_V_allheaders_h                             <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/allheaders.h>

#define BOSS_OPENALPR_U_tesseract__baseapi_h                     "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/include/tesseract/baseapi.h"
#define BOSS_OPENALPR_U_allheaders_h                             "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/allheaders.h"
#define BOSS_OPENALPR_U_alltypes_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/alltypes.h"
#define BOSS_OPENALPR_U_array_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/array.h"
#define BOSS_OPENALPR_U_arrayaccess_h                            "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/arrayaccess.h"
#define BOSS_OPENALPR_U_bbuffer_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/bbuffer.h"
#define BOSS_OPENALPR_U_bilateral_h                              "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/bilateral.h"
#define BOSS_OPENALPR_U_bmf_h                                    "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/bmf.h"
#define BOSS_OPENALPR_U_bmp_h                                    "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/bmp.h"
#define BOSS_OPENALPR_U_ccbord_h                                 "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/ccbord.h"
#define BOSS_OPENALPR_U_dewarp_h                                 "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/dewarp.h"
#define BOSS_OPENALPR_U_endianness_h                             "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/endianness.h"
#define BOSS_OPENALPR_U_environ_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/environ.h"
#define BOSS_OPENALPR_U_freetype_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/freetype.h"
#define BOSS_OPENALPR_U_gplot_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/gplot.h"
#define BOSS_OPENALPR_U_heap_h                                   "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/heap.h"
#define BOSS_OPENALPR_U_imageio_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/imageio.h"
#define BOSS_OPENALPR_U_jbclass_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/jbclass.h"
#define BOSS_OPENALPR_U_leptwin_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/leptwin.h"
#define BOSS_OPENALPR_U_list_h                                   "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/list.h"
#define BOSS_OPENALPR_U_morph_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/morph.h"
#define BOSS_OPENALPR_U_pix_h                                    "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/pix.h"
#define BOSS_OPENALPR_U_ptra_h                                   "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/ptra.h"
#define BOSS_OPENALPR_U_queue_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/queue.h"
#define BOSS_OPENALPR_U_readbarcode_h                            "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/readbarcode.h"
#define BOSS_OPENALPR_U_recog_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/recog.h"
#define BOSS_OPENALPR_U_regutils_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/regutils.h"
#define BOSS_OPENALPR_U_stack_h                                  "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/stack.h"
#define BOSS_OPENALPR_U_sudoku_h                                 "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/sudoku.h"
#define BOSS_OPENALPR_U_watershed_h                              "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/watershed.h"

// libtiff for tesseract
#define BOSS_OPENALPR_U_t4_h                                     "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/t4.h"
#define BOSS_OPENALPR_U_tif_config_h                             "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tif_config.h"
#define BOSS_OPENALPR_U_tif_fax3_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tif_fax3.h"
#define BOSS_OPENALPR_U_tif_predict_h                            "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tif_predict.h"
#define BOSS_OPENALPR_U_tiff_h                                   "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tiff.h"
#define BOSS_OPENALPR_U_tiffconf_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tiffconf.h"
#define BOSS_OPENALPR_U_tiffio_h                                 "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tiffio.h"
#define BOSS_OPENALPR_U_tiffiop_h                                "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tiffiop.h"
#define BOSS_OPENALPR_U_tiffvers_h                               "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/tiffvers.h"
#define BOSS_OPENALPR_U_uvcode_h                                 "addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/include/uvcode.h"