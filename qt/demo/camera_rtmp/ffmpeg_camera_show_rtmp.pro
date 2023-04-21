#-------------------------------------------------
#
# Project created by QtCreator 2021-04-21T17:25:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ffmpeg_camera_show_rtmp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    capturevideothread.cpp

HEADERS  += mainwindow.h \
    capturevideothread.h \
    ffmpegheader.h

FORMS    += mainwindow.ui

INCLUDEPATH += E:\Documents\RTMP-push-stream-based-on-Qt-and-FFMPEG-main\lib\ffmpeg\include

LIBS += -L$$PWD/lib/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
