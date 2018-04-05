#include <boss.hpp>
#include "boss_addon.hpp"

#include <element/boss_rect.hpp>
#include <element/boss_vector.hpp>

// 링크옵션 /OPT:NOREF가 안되서 임시코드
bool __LINK_ADDON_AAC__();
bool __LINK_ADDON_ALPR__();
bool __LINK_ADDON_CURL__();
bool __LINK_ADDON_GIT__();
bool __LINK_ADDON_H264__();
bool __LINK_ADDON_JPG__();
bool __LINK_ADDON_OGG__();
bool __LINK_ADDON_OPENCV__();
bool __LINK_ADDON_TESSERACT__();
bool __LINK_ADDON_ZIP__();
static bool _ =
    __LINK_ADDON_AAC__() |
    __LINK_ADDON_ALPR__() |
    __LINK_ADDON_CURL__() |
    __LINK_ADDON_GIT__() |
    __LINK_ADDON_H264__() |
    __LINK_ADDON_JPG__() |
    __LINK_ADDON_OGG__() |
    __LINK_ADDON_OPENCV__() |
    __LINK_ADDON_TESSERACT__() |
    __LINK_ADDON_ZIP__();

namespace BOSS
{
    ////////////////////////////////////////////////////////////////////////////////
    static void Aac_Error() {BOSS_ASSERT("Aac애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Aac, Create, id_acc, return nullptr, sint32, sint32, sint32)
    BOSS_DEFINE_ADDON_FUNCTION(Aac, Release, void, return, id_acc)
    BOSS_DEFINE_ADDON_FUNCTION(Aac, EncodeTo, void, return, id_acc, bytes, sint32, id_flash, uint64)
    BOSS_DEFINE_ADDON_FUNCTION(Aac, SilenceTo, void, return, id_acc, id_flash, uint64)

    id_acc AddOn::Aac::Create(sint32 bitrate, sint32 channel, sint32 samplerate)
    {return Core_AddOn_Aac_Create()(bitrate, channel, samplerate);}

    void AddOn::Aac::Release(id_acc acc)
    {Core_AddOn_Aac_Release()(acc);}

    void AddOn::Aac::EncodeTo(id_acc acc, bytes pcm, sint32 length, id_flash flash, uint64 timems)
    {Core_AddOn_Aac_EncodeTo()(acc, pcm, length, flash, timems);}

    void AddOn::Aac::SilenceTo(id_acc acc, id_flash flash, uint64 timems)
    {Core_AddOn_Aac_SilenceTo()(acc, flash, timems);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Alpr_Error() {BOSS_ASSERT("Alpr애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Alpr, Create, id_alpr, return nullptr, chars, h_view)
    BOSS_DEFINE_ADDON_FUNCTION(Alpr, Release, void, return, id_alpr)
    BOSS_DEFINE_ADDON_FUNCTION(Alpr, Recognize, chars, return "", id_alpr, id_bitmap_read)
    BOSS_DEFINE_ADDON_FUNCTION(Alpr, Summary, chars, return "", chars)

    id_alpr AddOn::Alpr::Create(chars country, h_view listener)
    {return Core_AddOn_Alpr_Create()(country, listener);}

    void AddOn::Alpr::Release(id_alpr alpr)
    {Core_AddOn_Alpr_Release()(alpr);}

    chars AddOn::Alpr::Recognize(id_alpr alpr, id_bitmap_read bitmap)
    {return Core_AddOn_Alpr_Recognize()(alpr, bitmap);}

    chars AddOn::Alpr::Summary(chars recognizedtext)
    {return Core_AddOn_Alpr_Summary()(recognizedtext);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Curl_Error() {BOSS_ASSERT("Curl애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Curl, Create, id_curl, return nullptr, void)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, CreateForUser, id_curl, return nullptr, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, Clone, id_curl, return nullptr, id_curl)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, Release, void, return, id_curl)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, RequestString, chars, return "", id_curl, chars, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, RequestBytes, bytes, return nullptr, id_curl, chars, sint32*, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, RequestRedirectUrl, chars, return "", id_curl, chars, sint32, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, SendStream, void, return, id_curl, chars, CurlReadCB, payload)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpUpload, bool, return false, id_curl, chars, chars, buffer)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpDownload, buffer, return nullptr, id_curl, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpDelete, bool, return false, id_curl, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpCreateFolder, bool, return false, id_curl, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpDeleteFolder, bool, return false, id_curl, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Curl, FtpSearch, sint32, return -1, id_curl, chars, chars, AddOn::Curl::SearchCB, payload)

    id_curl AddOn::Curl::Create(void)
    {return Core_AddOn_Curl_Create()();}

    id_curl AddOn::Curl::CreateForUser(chars username, chars password)
    {return Core_AddOn_Curl_CreateForUser()(username, password);}

    id_curl AddOn::Curl::Clone(id_curl curl)
    {return Core_AddOn_Curl_Clone()(curl);}

    void AddOn::Curl::Release(id_curl curl)
    {Core_AddOn_Curl_Release()(curl);}

    chars AddOn::Curl::RequestString(id_curl curl, chars url, chars postdata, chars headerdata)
    {return Core_AddOn_Curl_RequestString()(curl, url, postdata, headerdata);}

    bytes AddOn::Curl::RequestBytes(id_curl curl, chars url, sint32* getsize, chars postdata, chars headerdata)
    {return Core_AddOn_Curl_RequestBytes()(curl, url, getsize, postdata, headerdata);}

    chars AddOn::Curl::RequestRedirectUrl(id_curl curl, chars url, sint32 successcode, chars postdata, chars headerdata)
    {return Core_AddOn_Curl_RequestRedirectUrl()(curl, url, successcode, postdata, headerdata);}

    void AddOn::Curl::SendStream(id_curl curl, chars url, CurlReadCB cb, payload data)
    {Core_AddOn_Curl_SendStream()(curl, url, cb, data);}

    bool AddOn::Curl::FtpUpload(id_curl curl, chars url, chars filename, buffer data)
    {return Core_AddOn_Curl_FtpUpload()(curl, url, filename, data);}

    buffer AddOn::Curl::FtpDownload(id_curl curl, chars url, chars filename)
    {return Core_AddOn_Curl_FtpDownload()(curl, url, filename);}

    bool AddOn::Curl::FtpDelete(id_curl curl, chars url, chars filename)
    {return Core_AddOn_Curl_FtpDelete()(curl, url, filename);}

    bool AddOn::Curl::FtpCreateFolder(id_curl curl, chars url, chars dirname)
    {return Core_AddOn_Curl_FtpCreateFolder()(curl, url, dirname);}

    bool AddOn::Curl::FtpDeleteFolder(id_curl curl, chars url, chars dirname)
    {return Core_AddOn_Curl_FtpDeleteFolder()(curl, url, dirname);}

    sint32 AddOn::Curl::FtpSearch(id_curl curl, chars url, chars dirname, SearchCB cb, payload data)
    {return Core_AddOn_Curl_FtpSearch()(curl, url, dirname, cb, data);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Git_Error() {BOSS_ASSERT("Git애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Git, Create, id_git, return nullptr, chars, chars, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Git, Release, void, return, id_git)
    BOSS_DEFINE_ADDON_FUNCTION(Git, Update, void, return, id_git, AddOn::Git::ProgressCB, payload)

    id_git AddOn::Git::Create(chars rootpath, chars sshname, chars id, chars password)
    {return Core_AddOn_Git_Create()(rootpath, sshname, id, password);}

    void AddOn::Git::Release(id_git git)
    {Core_AddOn_Git_Release()(git);}

    void AddOn::Git::Update(id_git git, ProgressCB cb, payload data)
    {Core_AddOn_Git_Update()(git, cb, data);}

    ////////////////////////////////////////////////////////////////////////////////
    static void H264_Error() {BOSS_ASSERT("H264애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(H264, Create, id_h264, return nullptr, sint32, sint32, bool)
    BOSS_DEFINE_ADDON_FUNCTION(H264, Release, void, return, id_h264)
    BOSS_DEFINE_ADDON_FUNCTION(H264, EncodeTo, void, return, id_h264, const uint32*, id_flash, uint64)

    id_h264 AddOn::H264::Create(sint32 width, sint32 height, bool fastmode)
    {return Core_AddOn_H264_Create()(width, height, fastmode);}

    void AddOn::H264::Release(id_h264 h264)
    {Core_AddOn_H264_Release()(h264);}

    void AddOn::H264::EncodeTo(id_h264 h264, const uint32* rgba, id_flash flash, uint64 timems)
    {Core_AddOn_H264_EncodeTo()(h264, rgba, flash, timems);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Jpg_Error() {BOSS_ASSERT("Jpg애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Jpg, Create, id_jpg, return nullptr, id_bitmap_read, sint32)
    BOSS_DEFINE_ADDON_FUNCTION(Jpg, Release, void, return, id_jpg)
    BOSS_DEFINE_ADDON_FUNCTION(Jpg, GetLength, sint32, return 0, id_jpg)
    BOSS_DEFINE_ADDON_FUNCTION(Jpg, GetBits, bytes, return nullptr, id_jpg)
    BOSS_DEFINE_ADDON_FUNCTION(Jpg, ToBmp, id_bitmap, return nullptr, bytes, sint32)

    id_jpg AddOn::Jpg::Create(id_bitmap_read bmp, sint32 quality)
    {return Core_AddOn_Jpg_Create()(bmp, quality);}

    void AddOn::Jpg::Release(id_jpg jpg)
    {Core_AddOn_Jpg_Release()(jpg);}

    sint32 AddOn::Jpg::GetLength(id_jpg jpg)
    {return Core_AddOn_Jpg_GetLength()(jpg);}

    bytes AddOn::Jpg::GetBits(id_jpg jpg)
    {return Core_AddOn_Jpg_GetBits()(jpg);}

    id_bitmap AddOn::Jpg::ToBmp(bytes jpg, sint32 length)
    {return Core_AddOn_Jpg_ToBmp()(jpg, length);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Ogg_Error() {BOSS_ASSERT("Ogg애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Ogg, ToPcmStream, void, return, id_file_read, PcmCreateCB, PcmWriteCB, payload)

    void AddOn::Ogg::ToPcmStream(id_file_read oggfile, PcmCreateCB ccb, PcmWriteCB wcb, payload data)
    {return Core_AddOn_Ogg_ToPcmStream()(oggfile, ccb, wcb, data);}

    ////////////////////////////////////////////////////////////////////////////////
    static void OpenCV_Error() {BOSS_ASSERT("OpenCV애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, Create, id_opencv, return nullptr, void)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, Release, void, return, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, Update, void, return, id_opencv, id_bitmap_read, double, double, sint32)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, GetPreprocessImage, id_bitmap, return nullptr, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, TrackingObject, void, return, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, GetObjectCount, sint32, return 0, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, GetObjectRect, rect128f, return Rect(), id_opencv, sint32)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, TrackingStick, void, return, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, GetStickCount, sint32, return 0, id_opencv)
    BOSS_DEFINE_ADDON_FUNCTION(OpenCV, GetStickLine, vector128f, return Vector(), id_opencv, sint32)

    id_opencv AddOn::OpenCV::Create(void)
    {return Core_AddOn_OpenCV_Create()();}

    void AddOn::OpenCV::Release(id_opencv opencv)
    {Core_AddOn_OpenCV_Release()(opencv);}

    void AddOn::OpenCV::Update(id_opencv opencv, id_bitmap_read bmp, double low, double high, sint32 aperture)
    {Core_AddOn_OpenCV_Update()(opencv, bmp, low, high, aperture);}

    id_bitmap AddOn::OpenCV::GetPreprocessImage(id_opencv opencv)
    {return Core_AddOn_OpenCV_GetPreprocessImage()(opencv);}

    void AddOn::OpenCV::TrackingObject(id_opencv opencv)
    {Core_AddOn_OpenCV_TrackingObject()(opencv);}

    sint32 AddOn::OpenCV::GetObjectCount(id_opencv opencv)
    {return Core_AddOn_OpenCV_GetObjectCount()(opencv);}

    rect128f AddOn::OpenCV::GetObjectRect(id_opencv opencv, sint32 i)
    {return Core_AddOn_OpenCV_GetObjectRect()(opencv, i);}

    void AddOn::OpenCV::TrackingStick(id_opencv opencv)
    {Core_AddOn_OpenCV_TrackingStick()(opencv);}

    sint32 AddOn::OpenCV::GetStickCount(id_opencv opencv)
    {return Core_AddOn_OpenCV_GetStickCount()(opencv);}

    vector128f AddOn::OpenCV::GetStickLine(id_opencv opencv, sint32 i)
    {return Core_AddOn_OpenCV_GetStickLine()(opencv, i);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Tesseract_Error() {BOSS_ASSERT("Tesseract애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Tesseract, Create, id_tesseract, return nullptr, chars, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Tesseract, Release, void, return, id_tesseract)
    BOSS_DEFINE_ADDON_FUNCTION(Tesseract, Training, void, return, id_tesseract, chars)

    id_tesseract AddOn::Tesseract::Create(chars tifpath, chars otherpath)
    {return Core_AddOn_Tesseract_Create()(tifpath, otherpath);}

    void AddOn::Tesseract::Release(id_tesseract tesseract)
    {Core_AddOn_Tesseract_Release()(tesseract);}

    void AddOn::Tesseract::Training(id_tesseract tesseract, chars workname)
    {Core_AddOn_Tesseract_Training()(tesseract, workname);}

    ////////////////////////////////////////////////////////////////////////////////
    static void Zip_Error() {BOSS_ASSERT("Zip애드온이 준비되지 않았습니다", false);}
    BOSS_DEFINE_ADDON_FUNCTION(Zip, Create, id_zip, return nullptr, bytes, sint32, sint32*, chars)
    BOSS_DEFINE_ADDON_FUNCTION(Zip, Release, void, return, id_zip)
    BOSS_DEFINE_ADDON_FUNCTION(Zip, ToFile, buffer, return nullptr, id_zip, sint32)
    BOSS_DEFINE_ADDON_FUNCTION(Zip, GetFileInfo, chars, return "", id_zip, sint32,
        bool*, uint64*, uint64*, uint64*, bool*, bool*, bool*, bool*)

    id_zip AddOn::Zip::Create(bytes zip, sint32 length, sint32* filecount, chars password)
    {return Core_AddOn_Zip_Create()(zip, length, filecount, password);}

    void AddOn::Zip::Release(id_zip zip)
    {Core_AddOn_Zip_Release()(zip);}

    buffer AddOn::Zip::ToFile(id_zip zip, sint32 fileindex)
    {return Core_AddOn_Zip_ToFile()(zip, fileindex);}

    chars AddOn::Zip::GetFileInfo(id_zip zip, sint32 fileindex,
        bool* isdir, uint64* ctime, uint64* mtime, uint64* atime,
        bool* archive, bool* hidden, bool* readonly, bool* system)
    {return Core_AddOn_Zip_GetFileInfo()(zip, fileindex,
        isdir, ctime, mtime, atime, archive, hidden, readonly, system);}
}
