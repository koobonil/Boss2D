QT += core
QT += gui
QT += network
QT += opengl
QT += widgets
QT += multimedia
QT += bluetooth
!ios: QT += serialport
android: QT += androidextras
win32-msvc*: QT += webenginewidgets

CONFIG += c++11
CONFIG += mobility
CONFIG += warn_off
MOBILITY += systeminfo
win32-msvc*: QMAKE_CFLAGS += /bigobj
win32-msvc*: QMAKE_CXXFLAGS += /bigobj
android: LIBS += -landroid
ios|macx: LIBS += -framework CoreFoundation
linux-g++: CONFIG += link_pkgconfig
linux-g++: PKGCONFIG += gtk+-3.0

INCLUDEPATH += ../../Boss2D/core
INCLUDEPATH += ../../Boss2D
CONFIG(debug, debug|release){
    DEFINES += BOSS_NDEBUG=0
}
CONFIG(release, debug|release){
    DEFINES += BOSS_NDEBUG=1
}

###########################################################
# ADDON
###########################################################
HEADERS += ../../Boss2D/addon/boss_addon_aac.hpp
HEADERS += ../../Boss2D/addon/boss_addon_alpr.hpp
HEADERS += ../../Boss2D/addon/boss_addon_curl.hpp
HEADERS += ../../Boss2D/addon/boss_addon_git.hpp
HEADERS += ../../Boss2D/addon/boss_addon_h264.hpp
HEADERS += ../../Boss2D/addon/boss_addon_jpg.hpp
HEADERS += ../../Boss2D/addon/boss_addon_tesseract.hpp
HEADERS += ../../Boss2D/addon/boss_addon_zip.hpp
HEADERS += ../../Boss2D/addon/boss_fakewin.h
HEADERS += ../../Boss2D/addon/boss_integration_curl-7.51.0.h
HEADERS += ../../Boss2D/addon/boss_integration_fdk-aac-0.1.4.h
HEADERS += ../../Boss2D/addon/boss_integration_libgit2-0.22.2.h
HEADERS += ../../Boss2D/addon/boss_integration_libssh2-1.6.0.h
HEADERS += ../../Boss2D/addon/boss_integration_openalpr-2.3.0.h
HEADERS += ../../Boss2D/addon/boss_integration_opencv-3.1.0.h
HEADERS += ../../Boss2D/addon/boss_integration_opencv-3.1.0_3rdparty_libjpeg.h
HEADERS += ../../Boss2D/addon/boss_integration_openh264-1.6.0.h
HEADERS += ../../Boss2D/addon/boss_integration_openssl-1.1.0c.h
HEADERS += ../../Boss2D/addon/boss_integration_rtmpdump-2.4.h
HEADERS += ../../Boss2D/addon/boss_integration_tesseract-3.04.01.h
SOURCES += ../../Boss2D/addon/boss_addon_aac.cpp
SOURCES += ../../Boss2D/addon/boss_addon_alpr.cpp
SOURCES += ../../Boss2D/addon/boss_addon_curl.cpp
SOURCES += ../../Boss2D/addon/boss_addon_git.cpp
SOURCES += ../../Boss2D/addon/boss_addon_h264.cpp
SOURCES += ../../Boss2D/addon/boss_addon_jpg.cpp
SOURCES += ../../Boss2D/addon/boss_addon_tesseract.cpp
SOURCES += ../../Boss2D/addon/boss_addon_zip.cpp
SOURCES += ../../Boss2D/addon/boss_fakewin.cpp
SOURCES += ../../Boss2D/addon/boss_integration_curl-7.51.0.c
SOURCES += ../../Boss2D/addon/boss_integration_fdk-aac-0.1.4.cpp
SOURCES += ../../Boss2D/addon/boss_integration_libgit2-0.22.2.c
SOURCES += ../../Boss2D/addon/boss_integration_libssh2-1.6.0.c
SOURCES += ../../Boss2D/addon/boss_integration_openalpr-2.3.0.cpp
SOURCES += ../../Boss2D/addon/boss_integration_opencv-3.1.0.cpp
SOURCES += ../../Boss2D/addon/boss_integration_opencv-3.1.0_3rdparty_libjpeg.c
SOURCES += ../../Boss2D/addon/boss_integration_openh264-1.6.0.cpp
SOURCES += ../../Boss2D/addon/boss_integration_openssl-1.1.0c.c
SOURCES += ../../Boss2D/addon/boss_integration_rtmpdump-2.4.c
SOURCES += ../../Boss2D/addon/boss_integration_tesseract-3.04.01.cpp

###########################################################
# CORE
###########################################################
HEADERS += ../../Boss2D/core/boss.h
HEADERS += ../../Boss2D/core/boss.hpp
HEADERS += ../../Boss2D/core/boss_addon.hpp
HEADERS += ../../Boss2D/core/boss_array.hpp
HEADERS += ../../Boss2D/core/boss_asset.hpp
HEADERS += ../../Boss2D/core/boss_buffer.hpp
HEADERS += ../../Boss2D/core/boss_context.hpp
HEADERS += ../../Boss2D/core/boss_counter.hpp
HEADERS += ../../Boss2D/core/boss_library.hpp
HEADERS += ../../Boss2D/core/boss_map.hpp
HEADERS += ../../Boss2D/core/boss_math.hpp
HEADERS += ../../Boss2D/core/boss_memory.hpp
HEADERS += ../../Boss2D/core/boss_mutex.hpp
HEADERS += ../../Boss2D/core/boss_object.hpp
HEADERS += ../../Boss2D/core/boss_parser.hpp
HEADERS += ../../Boss2D/core/boss_profile.hpp
HEADERS += ../../Boss2D/core/boss_queue.hpp
HEADERS += ../../Boss2D/core/boss_remote.hpp
HEADERS += ../../Boss2D/core/boss_share.hpp
HEADERS += ../../Boss2D/core/boss_storage.hpp
HEADERS += ../../Boss2D/core/boss_string.hpp
HEADERS += ../../Boss2D/core/boss_tasking.hpp
HEADERS += ../../Boss2D/core/boss_updater.hpp
HEADERS += ../../Boss2D/core/boss_view.hpp
HEADERS += ../../Boss2D/core/boss_wstring.hpp
SOURCES += ../../Boss2D/core/boss.cpp
SOURCES += ../../Boss2D/core/boss_addon.cpp
SOURCES += ../../Boss2D/core/boss_asset.cpp
SOURCES += ../../Boss2D/core/boss_buffer.cpp
SOURCES += ../../Boss2D/core/boss_context.cpp
SOURCES += ../../Boss2D/core/boss_library.cpp
SOURCES += ../../Boss2D/core/boss_math.cpp
SOURCES += ../../Boss2D/core/boss_memory.cpp
SOURCES += ../../Boss2D/core/boss_mutex.cpp
SOURCES += ../../Boss2D/core/boss_parser.cpp
SOURCES += ../../Boss2D/core/boss_profile.cpp
SOURCES += ../../Boss2D/core/boss_remote.cpp
SOURCES += ../../Boss2D/core/boss_share.cpp
SOURCES += ../../Boss2D/core/boss_storage.cpp
SOURCES += ../../Boss2D/core/boss_string.cpp
SOURCES += ../../Boss2D/core/boss_tasking.cpp
SOURCES += ../../Boss2D/core/boss_updater.cpp
SOURCES += ../../Boss2D/core/boss_view.cpp
SOURCES += ../../Boss2D/core/boss_wstring.cpp

###########################################################
# ELEMENT
###########################################################
HEADERS += ../../Boss2D/element/boss_clip.hpp
HEADERS += ../../Boss2D/element/boss_color.hpp
HEADERS += ../../Boss2D/element/boss_font.hpp
HEADERS += ../../Boss2D/element/boss_image.hpp
HEADERS += ../../Boss2D/element/boss_matrix.hpp
HEADERS += ../../Boss2D/element/boss_point.hpp
HEADERS += ../../Boss2D/element/boss_rect.hpp
HEADERS += ../../Boss2D/element/boss_size.hpp
HEADERS += ../../Boss2D/element/boss_solver.hpp
HEADERS += ../../Boss2D/element/boss_tween.hpp
HEADERS += ../../Boss2D/element/boss_vector.hpp
SOURCES += ../../Boss2D/element/boss_clip.cpp
SOURCES += ../../Boss2D/element/boss_color.cpp
SOURCES += ../../Boss2D/element/boss_font.cpp
SOURCES += ../../Boss2D/element/boss_image.cpp
SOURCES += ../../Boss2D/element/boss_matrix.cpp
SOURCES += ../../Boss2D/element/boss_point.cpp
SOURCES += ../../Boss2D/element/boss_rect.cpp
SOURCES += ../../Boss2D/element/boss_size.cpp
SOURCES += ../../Boss2D/element/boss_solver.cpp
SOURCES += ../../Boss2D/element/boss_tween.cpp
SOURCES += ../../Boss2D/element/boss_vector.cpp

###########################################################
# FORMAT
###########################################################
HEADERS += ../../Boss2D/format/boss_bmp.hpp
HEADERS += ../../Boss2D/format/boss_flv.hpp
HEADERS += ../../Boss2D/format/boss_png.hpp
HEADERS += ../../Boss2D/format/boss_zen.hpp
SOURCES += ../../Boss2D/format/boss_bmp.cpp
SOURCES += ../../Boss2D/format/boss_flv.cpp
SOURCES += ../../Boss2D/format/boss_png.cpp
SOURCES += ../../Boss2D/format/boss_zen.cpp

###########################################################
# PLATFORM
###########################################################
HEADERS += ../../Boss2D/platform/boss_platform.hpp
HEADERS += ../../Boss2D/platform/boss_platform_blank.hpp
HEADERS += ../../Boss2D/platform/boss_platform_cocos2dx.hpp
HEADERS += ../../Boss2D/platform/boss_platform_impl.hpp
HEADERS += ../../Boss2D/platform/boss_platform_native.hpp
HEADERS += ../../Boss2D/platform/boss_platform_qt5.hpp
SOURCES += ../../Boss2D/platform/boss_platform_blank.cpp
SOURCES += ../../Boss2D/platform/boss_platform_cocos2dx.cpp
SOURCES += ../../Boss2D/platform/boss_platform_impl.cpp
SOURCES += ../../Boss2D/platform/boss_platform_native.cpp
SOURCES += ../../Boss2D/platform/boss_platform_qt5.cpp
HEADERS += ../../Boss2D/platform/android/OpenGLES_Functions.h
SOURCES += ../../Boss2D/platform/android/OpenGLES_Functions.cpp
HEADERS += ../../Boss2D/platform/win32/glew.h

###########################################################
# SERVICE
###########################################################
HEADERS += ../../Boss2D/service/boss_asyncfilemanager.hpp
HEADERS += ../../Boss2D/service/boss_backey.hpp
HEADERS += ../../Boss2D/service/boss_batchbuilder.hpp
HEADERS += ../../Boss2D/service/boss_boxr.hpp
HEADERS += ../../Boss2D/service/boss_brii.hpp
HEADERS += ../../Boss2D/service/boss_camera360service.hpp
HEADERS += ../../Boss2D/service/boss_livechatservice.hpp
HEADERS += ../../Boss2D/service/boss_oauth2service.hpp
HEADERS += ../../Boss2D/service/boss_streamingservice.hpp
HEADERS += ../../Boss2D/service/boss_tryworld.hpp
HEADERS += ../../Boss2D/service/boss_zay.hpp
SOURCES += ../../Boss2D/service/boss_asyncfilemanager.cpp
SOURCES += ../../Boss2D/service/boss_backey.cpp
SOURCES += ../../Boss2D/service/boss_batchbuilder.cpp
SOURCES += ../../Boss2D/service/boss_boxr.cpp
SOURCES += ../../Boss2D/service/boss_brii.cpp
SOURCES += ../../Boss2D/service/boss_camera360service.cpp
SOURCES += ../../Boss2D/service/boss_livechatservice.cpp
SOURCES += ../../Boss2D/service/boss_oauth2service.cpp
SOURCES += ../../Boss2D/service/boss_streamingservice.cpp
SOURCES += ../../Boss2D/service/boss_tryworld.cpp
SOURCES += ../../Boss2D/service/boss_zay.cpp
