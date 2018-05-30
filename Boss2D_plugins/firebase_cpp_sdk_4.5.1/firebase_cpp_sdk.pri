QT += core
QT += gui
QT += network
QT += widgets
android: QT += androidextras

DEFINES += BOSS_PLUGINS_FIREBASE
INCLUDEPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/include

win32-msvc*:{
    CONFIG(debug, debug|release){
        contains(QT_ARCH, i386){
            LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/windows/VS2015/MD/x86/Debug
        } else {
            LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/windows/VS2015/MD/x64/Debug
        }
    } else {
        contains(QT_ARCH, i386){
            LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/windows/VS2015/MD/x86/Release
        } else {
            LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/windows/VS2015/MD/x64/Release
        }
    }
    LIBS += firebase_app.lib
    LIBS += firebase_admob.lib
}

macx:{
    LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/darwin/universal
    LIBS += -lfirebase_app
    LIBS += -lfirebase_admob

    QMAKE_LFLAGS += -F../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/frameworks/darwin/universal
    LIBS += -framework firebase
    LIBS += -framework firebase_admob
}

ios:{
    !exists($$OUT_PWD/../common/ios/GoogleService-Info.plist) {
        message("BOSS_PLUGINS_FIREBASE : File(GoogleService-Info.plist) not found!!!")
        message("BOSS_PLUGINS_FIREBASE : Please refer to this folder -> Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk_example_common/ios")
        message("BOSS_PLUGINS_FIREBASE : See also -> https://firebase.google.com/")
    }
    GOOGLE_SERVICE_INFO.files += $$OUT_PWD/../common/ios/GoogleService-Info.plist
    GOOGLE_SERVICE_INFO.path = /
    QMAKE_BUNDLE_DATA += GOOGLE_SERVICE_INFO

    LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/ios/universal
    LIBS += -lfirebase_app
    LIBS += -lfirebase_admob

    LIBS += -framework AdSupport
    LIBS += -framework StoreKit
    LIBS += -framework MediaPlayer
    LIBS += -framework CoreMotion
    LIBS += -framework CoreTelephony
    LIBS += -framework MessageUI
    LIBS += -framework GLKit
    LIBS += -framework AddressBook
    LIBS += -framework UIKit
    LIBS += -framework SystemConfiguration
    LIBS += -framework CFNetwork

    QMAKE_LFLAGS += -F../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/frameworks/ios/universal
    LIBS += -framework firebase
    LIBS += -framework firebase_admob
    QMAKE_LFLAGS += -F../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk_ios_frameworks
    LIBS += -framework FirebaseCore
    LIBS += -framework GoogleMobileAds
}

android:{
    !exists($$OUT_PWD/../common/android/build.gradle) {
        message("BOSS_PLUGINS_FIREBASE : File(build.gradle) not found!!!")
        message("BOSS_PLUGINS_FIREBASE : Please refer to this folder -> Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk_example_common/android")
    }
    !exists($$OUT_PWD/../common/android/google-services.json) {
        message("BOSS_PLUGINS_FIREBASE : File(google-services.json) not found!!!")
        message("BOSS_PLUGINS_FIREBASE : Please refer to this folder -> Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk_example_common/android")
        message("BOSS_PLUGINS_FIREBASE : See also -> https://firebase.google.com/")
    }
    !exists($$OUT_PWD/../common/android/res/drawable-hdpi-v4/common_google_signin_btn_icon_dark_normal_background.9.png) {
        message("BOSS_PLUGINS_FIREBASE : Missing required resource folder!!!")
        message("BOSS_PLUGINS_FIREBASE : Please refer to this folder -> Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk_example_common/android/res")
    }
    LIBPATH += ../../Boss2D_plugins/firebase_cpp_sdk_4.5.1/firebase_cpp_sdk/libs/android/armeabi-v7a/gnustl
    LIBS += -lapp
    LIBS += -ladmob
}
