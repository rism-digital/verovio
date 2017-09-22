QT -= gui
QT += quick

CONFIG += c++14

TARGET = verovio-qt
TEMPLATE = lib

DEFINES += VEROVIOQT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/qtscenegraphdevicecontext.cpp \
    src/textquickitem.cpp \
    src/veroviopage.cpp \
    src/veroviodoc.cpp

HEADERS +=\
    include/verovio-qt/qtscenegraphdevicecontext.h \
    include/verovio-qt/textquickitem.h \
    include/verovio-qt/veroviopage.h \
    include/verovio-qt/veroviodoc.h

INCLUDEPATH += include/verovio-qt

{
    INCLUDEPATH += ../../include/vrv
    INCLUDEPATH += ../../include/pugi
    INCLUDEPATH += ../../include/utf8
    INCLUDEPATH += ../../libmei
    LIBS += -L../../build -lverovio
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
