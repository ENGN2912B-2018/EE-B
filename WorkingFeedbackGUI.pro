#-------------------------------------------------
#
# Project created by QtCreator 2018-12-16T18:43:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkingFeedbackGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        analyzerwindow.cpp \
    src/FeedbackID.cpp \
    src/FileReader.cpp \
    kiss_fft130/tools/kiss_fftr.c \
    kiss_fft130/kiss_fft.c \

HEADERS += \
        analyzerwindow.h \
    kiss_fft130/tools/kfc.h \
    kiss_fft130/tools/kiss_fftnd.h \
    kiss_fft130/tools/kiss_fftndr.h \
    kiss_fft130/tools/kiss_fftr.h \
    kiss_fft130/_kiss_fft_guts.h \
    kiss_fft130/kiss_fft.h \
    kiss_fft130/kissfft.hh \
    src/FeedbackID.h \
    src/FFTAnalyzer.h \
    src/FileReader.h \

FORMS += \
        analyzerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    kiss_fft130/tools/fastconv_float \
    kiss_fft130/tools/fastconv_simd \
    kiss_fft130/tools/fastconvr_float \
    kiss_fft130/tools/fastconvr_simd \
    kiss_fft130/tools/fft_float \
    kiss_fft130/tools/fft_simd \
    wavfiles/example1.wav \
    wavfiles/example2.wav \
    kiss_fft130/.hg_archival.txt \
    kiss_fft130/.hgignore \
    kiss_fft130/.hgtags \
    kiss_fft130/TIPS \
    kiss_fft130/CHANGELOG \
    kiss_fft130/CMakeLists.txt \
    src/CMakeLists.txt \
    kiss_fft130/COPYING \
    kiss_fft130/README \
    kiss_fft130/README.simd
