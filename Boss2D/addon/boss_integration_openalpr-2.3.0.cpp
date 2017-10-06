#include <boss.h>
#if BOSS_NEED_ADDON_ALPR

#include <iostream>
#include <functional>
#include <cctype>

#if BOSS_WINDOWS
    #pragma comment(lib, "gdi32.lib")
#endif

// 도구준비
#include <addon/boss_fakewin.h>
#include "boss_integration_openalpr-2.3.0.h"

#pragma warning(disable : 4305)
#define OPENALPR_MAJOR_VERSION "2"
#define OPENALPR_MINOR_VERSION "3"
#define OPENALPR_PATCH_VERSION "0"
#define NO_THREADS
#if BOSS_WINDOWS && !BOSS_NEED_FORCED_FAKEWIN
    #define INSTALL_PREFIX "../assets"
#else
    #define INSTALL_PREFIX "assets:"
#endif

#include <time.h>
#if _MSC_VER >= 1900
    #if BOSS_X64
        #define timespec timespec64_openalpr_BOSS
        typedef long long time64_t_openalpr_BOSS;
        struct timespec64_openalpr_BOSS
        {
            time64_t_openalpr_BOSS tv_sec;
            long tv_nsec;
        };
    #else
        #define timespec timespec32_openalpr_BOSS
        #ifdef __mips__
            typedef    long long time32_t_openalpr_BOSS;
        #else
            typedef    int time32_t_openalpr_BOSS;
        #endif
        struct timespec32_openalpr_BOSS
        {
            time32_t_openalpr_BOSS tv_sec;
            int tv_nsec;
        };
    #endif
#endif

#define sprintf_s sprintf_s_openalpr_BOSS
extern "C" int sprintf_s_openalpr_BOSS(char* str, int strlen, const char* format, ...)
{
    va_list Args;
    va_start(Args, format);
    int Result = boss_vsnprintf(str, strlen, format, Args);
    va_end(Args);
    return Result;
}

extern "C"
{
    void AlprDebug_AddRect(int x, int y, int w, int h);
    void AlprDebug_Flush(const char* subject, const char* comment);
}

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/alpr.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/alpr_impl.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/config.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/config_helper.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/binarize_wolf.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/cjson.c>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/colorfilter.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/licenseplatecandidate.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/motiondetector.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/pipeline_data.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/prewarp.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/result_aggregator.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/transformation.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/utility.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detector.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectorcpu.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectorcuda.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectorfactory.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectormask.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectormorph.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/detection/detectorocl.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/edges/edgefinder.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/edges/platecorners.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/edges/platelines.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/edges/scorekeeper.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/edges/textlinecollection.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/postprocess/postprocess.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/postprocess/regexrule.cpp>

extern "C"
{
    #include <addon/openalpr-2.3.0_for_boss/src/openalpr/simpleini/ConvertUTF.c>
}
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/simpleini/snippets.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/characteranalysis.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/linefinder.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/platemask.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/textcontours.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/textdetection/textline.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/ocr.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/ocrfactory.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/tesseract_ocr.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/segmentation/histogram.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/segmentation/histogramhorizontal.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/segmentation/charactersegmenter.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/ocr/segmentation/histogramvertical.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/statedetection/featurematcher.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/statedetection/line_segment.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/statedetection/state_detector.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/statedetection/state_detector_impl.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/filesystem.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/platform.cpp>
#define clock_gettime clock_gettime_timing_BOSS
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/timing.cpp>
#undef clock_gettime
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/tinythread.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/utf8.cpp>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/version.cpp>

#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/bitstate.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/compile.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/dfa.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/filtered_re2.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/mimics_pcre.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/nfa.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/onepass.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/parse.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/perl_groups.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prefilter.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prefilter_tree.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/prog.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/re2.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/regexp.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/set.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/simplify.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/stringpiece.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/tostring.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/unicode_casefold.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/unicode_groups.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/hash.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/rune.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/stringprintf.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/strutil.cc>
#include <addon/openalpr-2.3.0_for_boss/src/openalpr/support/re2/util/valgrind.cc>

// define for tesseract
#define int8 int8_tesseract
#define uint8 uint8_tesseract
#define int16 int16_tesseract
#define uint16 uint16_tesseract
#define int32 int32_tesseract
#define uint32 uint32_tesseract

// liblept for tesseract
extern "C"
{
    #pragma warning(disable : 4838)
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/include/environ.h>
    #define HAVE_LIBJPEG  0
    #define HAVE_LIBTIFF  1
    #define HAVE_LIBPNG   0
    #define HAVE_LIBZ     0
    #define HAVE_LIBGIF   0
    #define HAVE_LIBUNGIF 0
    #define HAVE_LIBWEBP  0

    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/adaptmap.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/affine.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/affinecompose.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/arrayaccess.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bardecode.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/baseline.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bbuffer.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bilateral.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bilinear.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/binarize.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/binexpand.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/binreduce.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/blend.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bmf.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bmpio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bmpiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/boxbasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/boxfunc1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/boxfunc2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/boxfunc3.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/boxfunc4.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/bytearray.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_ccbord_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ccbord.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ccthin.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/classapp.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colorcontent.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/coloring.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colormap.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colormorph.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colorquant1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colorquant2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colorseg.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/colorspace.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/compare.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/conncomp.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/convertfiles.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/convolve.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/convolvelow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/correlscore.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_dewarp1_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dewarp1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dewarp2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dewarp3.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dewarp4.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_dnabasic_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dnabasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dwacomb.2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/dwacomblow.2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/edge.c>
    // main()이 있음: #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/endiantest.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/enhance.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fhmtauto.c>
    #define NUM_SELS_GENERATED NUM_SELS_GENERATED_fhmtgen_1_BOSS
    #define SEL_NAMES SEL_NAMES_fhmtgen_1_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fhmtgen.1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fhmtgenlow.1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/finditalic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/flipdetect.c>
    #define NUM_SELS_GENERATED NUM_SELS_GENERATED_fliphmtgen_BOSS
    #define SEL_NAMES SEL_NAMES_fliphmtgen_BOSS
    #define fhmt_1_0 fhmt_1_0_fliphmtgen_BOSS
    #define fhmt_1_1 fhmt_1_1_fliphmtgen_BOSS
    #define fhmt_1_2 fhmt_1_2_fliphmtgen_BOSS
    #define fhmt_1_3 fhmt_1_3_fliphmtgen_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fliphmtgen.c>
    #define wpldecls wpldecls_fmorphauto_BOSS
    #define wpldefs wpldefs_fmorphauto_BOSS
    #define wplstrp wplstrp_fmorphauto_BOSS
    #define wplstrm wplstrm_fmorphauto_BOSS
    #define makeBarrelshiftString makeBarrelshiftString_fmorphauto_BOSS
    #define sarrayMakeInnerLoopDWACode sarrayMakeInnerLoopDWACode_fmorphauto_BOSS
    #define sarrayMakeWplsCode sarrayMakeWplsCode_fmorphauto_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fmorphauto.c>
    #define NUM_SELS_GENERATED NUM_SELS_GENERATED_fmorphgen_1_BOSS
    #define SEL_NAMES SEL_NAMES_fmorphgen_1_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fmorphgen.1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fmorphgenlow.1.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_fpix1_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fpix1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/fpix2.c>
    //#include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/freetype.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/gifio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/gifiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/gplot.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/graphics.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/graymorph.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/graymorphlow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/grayquant.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/grayquantlow.c>
    #define INITIAL_BUFFER_ARRAYSIZE INITIAL_BUFFER_ARRAYSIZE_heap_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/heap.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/jbclass.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/jp2kio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/jp2kiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/jpegio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/jpegiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/kernel.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/leptwin.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/libversions.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/list.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/maze.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/morph.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/morphapp.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/morphdwa.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/morphseq.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_numabasic_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/numabasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/numafunc1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/numafunc2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pageseg.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/paintcmap.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/parseprotos.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/partition.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pdfio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pdfiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pix1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pix2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pix3.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pix4.c>
    #define rmask32 rmask32_pix5_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pix5.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_pixabasic_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixabasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixacc.c>
    #define MIN_COMPS_FOR_BIN_SORT MIN_COMPS_FOR_BIN_SORT_pixafunc1_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixafunc1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixafunc2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixalloc.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixarith.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_pixcomp_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixcomp.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixconv.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixlabel.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pixtiling.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pngio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pngiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pnmio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/pnmiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/projective.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/psio1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/psio1stub.c>
    #define DEFAULT_INPUT_RES DEFAULT_INPUT_RES_psio2_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/psio2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/psio2stub.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_ptabasic_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ptabasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ptafunc1.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_ptra_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ptra.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/quadtree.c>
    #define MIN_BUFFER_SIZE MIN_BUFFER_SIZE_queue_BOSS
    #define INITIAL_BUFFER_ARRAYSIZE INITIAL_BUFFER_ARRAYSIZEqueue__BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/queue.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rank.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/readbarcode.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/readfile.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_recogbasic_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/recogbasic.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/recogdid.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/recogident.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/recogtrain.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/regutils.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rop.c>
    #define rmask32 rmask32_ropiplow_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/ropiplow.c>
    #define lmask32 lmask32_roplow_BOSS
    #define rmask32 rmask32_roplow_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/roplow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotate.c>
    #define MIN_ANGLE_TO_ROTATE MIN_ANGLE_TO_ROTATE_rotateam_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotateam.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotateamlow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotateorth.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotateorthlow.c>
    #define MIN_ANGLE_TO_ROTATE MIN_ANGLE_TO_ROTATE_rotateshear_BOSS
    #define LIMIT_SHEAR_ANGLE LIMIT_SHEAR_ANGLE_rotateshear_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/rotateshear.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/runlength.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/sarray.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/scale.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/scalelow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/seedfill.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/seedfilllow.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/sel1.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/sel2.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/selgen.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/shear.c>
    #define DEFAULT_SWEEP_RANGE DEFAULT_SWEEP_RANGE_skew_BOSS
    #define DEFAULT_SWEEP_DELTA DEFAULT_SWEEP_DELTA_skew_BOSS
    #define DEFAULT_MINBS_DELTA DEFAULT_MINBS_DELTA_skew_BOSS
    #define DEFAULT_SWEEP_REDUCTION DEFAULT_SWEEP_REDUCTION_skew_BOSS
    #define DEFAULT_BS_REDUCTION DEFAULT_BS_REDUCTION_skew_BOSS
    #define MIN_ALLOWED_CONFIDENCE MIN_ALLOWED_CONFIDENCE_skew_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/skew.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/spixio.c>
    #define INITIAL_PTR_ARRAYSIZE INITIAL_PTR_ARRAYSIZE_stack_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/stack.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/sudoku.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/textops.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/tiffio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/tiffiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/utils.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/viewfiles.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/warper.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/watershed.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/webpio.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/webpiostub.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/writefile.c>
    // main()이 있음: #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/xtractprotos.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/zlibmem.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/liblept/src/zlibmemstub.c>
}

// libtiff for tesseract
extern "C"
{
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_aux.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_close.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_codec.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_color.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_compress.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_dir.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_dirinfo.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_dirread.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_dirwrite.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_dumpmode.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_error.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_extension.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_fax3.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_fax3sm.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_flush.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_getimage.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_jbig.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_jpeg.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_luv.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_lzw.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_next.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_ojpeg.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_open.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_packbits.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_pixarlog.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_predict.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_print.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_read.c>
    #define summarize summarize_tif_strip_BOSS
    #define multiply multiply_tif_strip_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_strip.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_swab.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_thunder.c>
    #define summarize summarize_tif_tile_BOSS
    #define multiply multiply_tif_tile_BOSS
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_tile.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_version.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_warning.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_write.c>
    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_zip.c>

    #include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_msdos.c>
    //#include <addon/openalpr-2.3.0_for_boss/openalpr-windows-2.2.0/tesseract-ocr/dependencies/libtiff/libtiff/src/tif_unix.c>
}

#endif
