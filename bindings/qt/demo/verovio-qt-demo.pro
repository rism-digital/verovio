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

{
    INCLUDEPATH += ../../../include/
    INCLUDEPATH += ../../../include/vrv
    INCLUDEPATH += ../../../include/pugi
    INCLUDEPATH += ../../../include/utf8
    INCLUDEPATH += ../../../libmei
    LIBS += -L../../../tools -lverovio
}

{
    INCLUDEPATH += ../../qt/lib/include
    LIBS += -L../../qt/build-library -lverovio-qt
}
