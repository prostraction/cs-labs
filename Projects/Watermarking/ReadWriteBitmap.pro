QT	    += core gui
QT	    += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += optimize_full

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bytesencoding.cpp \
    codingmath.cpp \
    fileio.cpp \
    main.cpp \
    mainwindow.cpp \
    ui_tablemodel.cpp \
    videoio.cpp \
    watermark.cpp \
    watermarkembedding.cpp \
    watermarkimage.cpp

HEADERS += \
    bytesencoding.h \
    codingmath.h \
    fileio.h \
    mainwindow.h \
    ui_tablemodel.h \
    videoio.h \
    watermark.h \
    watermarkembedding.h \
    watermarkimage.h

FORMS += \
    mainwindow.ui

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE += -O2

#INCLUDEPATH += -L'$$_PRO_FILE_PWD_\..\ffmpeg_x64\include'
#LIBS	    += -L'$$_PRO_FILE_PWD_\..\ffmpeg_x64\lib' -lavdevice -lopenh264


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L'$$PWD/../ffmpeg_x64/lib/' -llibx264 -lavformat  -lavcodec -lavutil -lswscale -lavutil -lswresample

INCLUDEPATH += $$PWD/../ffmpeg_x64/include
DEPENDPATH += $$PWD/../ffmpeg_x64/include
