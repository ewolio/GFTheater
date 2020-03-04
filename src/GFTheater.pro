QT += quick gui widgets quickwidgets
CONFIG += c++11

SOURCES += main.cpp
SOURCES += $$files(ui/*.cpp, true)
SOURCES += $$files(model/*.cpp, true)

HEADERS += $$files(ui/*.h, true)
HEADERS += $$files(model/*.h, true)

FORMS += $$files(ui/*.ui, true)
RESOURCES += ui/qml.qrc \
    ../rsrc/genericRsrc.qrc \
    ui/qdarkstyle/style.qrc

LIB = $$PWD/../lib

INCLUDEPATH += $$LIB/include
LIBS += -L$$LIB\build\quazip\win64 -lquazip
LIBS += -L$$LIB\build\rtmidi\win64 -lrtmidi

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
