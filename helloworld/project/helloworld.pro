TARGET = helloworld
TEMPLATE = app

!include(../../Boss2D/project/boss2d.pri) {
    error("Couldn't find the boss2d.pri file...")
}

INCLUDEPATH += ../source
HEADERS += ../source/boss_config.h
SOURCES += ../source/helloworld.cpp
HEADERS += ../source/helloworld.hpp
SOURCES += ../source/main.cpp
SOURCES += ../source/resource.cpp
HEADERS += ../source/resource.hpp

ASSETS_IMAGE.files += ../assets/image
ASSETS_IMAGE.path = /assets

win32{
    RC_ICONS += ../common/windows/main.ico
}

android{
    INSTALLS += ASSETS_IMAGE
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../common/android
}

ios|macx{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_BUNDLE_DATA += PLIST_RESOURCE
    QMAKE_INFO_PLIST = $$PWD/../common/ios/Info.plist
    PLIST_RESOURCE.files += $$PWD/../common/ios/res/icon.icns
    ios: PLIST_RESOURCE.path = /
    macx: PLIST_RESOURCE.path = /Contents/Resources
}
