QT += qml quick

CONFIG += c++14
TARGET = verovio-qt-demo

SOURCES += main.cpp

HEADERS +=

RESOURCES += qml.qrc verovio.qrc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

GIT_BASE_PATH = $$shell_path($$_PRO_FILE_PWD_)/../../..

{
    INCLUDEPATH += ../../../include/
    INCLUDEPATH += ../../../include/vrv
    INCLUDEPATH += ../../../include/pugi
    INCLUDEPATH += ../../../include/utf8
    INCLUDEPATH += ../../../libmei
    INCLUDEPATH += ../lib/include
}

android {
    message("* Using settings for Android.")

    LIBS += -L../../../bindings/android/libs/armeabi-v7a -lverovio-android
    LIBS += -L../build-android -lverovio-qt_armeabi-v7a

    ANDROID_EXTRA_LIBS += "$${GIT_BASE_PATH}/bindings/android/libs/armeabi-v7a/libverovio-android.so"
    ANDROID_EXTRA_LIBS += "$${GIT_BASE_PATH}/bindings/qt/build-android/libverovio-qt_armeabi-v7a.so"

    # The 'data' directory with the font resources is put into the assets of the Android APK
    resources_data.path = /assets/data
    resources_data.files = $$files($$PWD/../../../data/*)
    INSTALLS += resources_data
}
else {
    message("* Using settings for Other (Linux, Mac, Windows, ...).")

    LIBS += -L../build-library -lverovio-qt
    LIBS += -L../../../tools -lverovio
}

