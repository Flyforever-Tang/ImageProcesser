#-------------------------------------------------
#
# Project created by QtCreator 2019-09-30T15:46:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcesser
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
        binaryImageBox.cpp \
        function.cpp \
        grayImageProcesser.cpp \
        grayImageStatiscian.cpp \
        grayimage.cpp \
        huffmanTreeChart.cpp \
        imageControler.cpp \
        imageTransshaper.cpp \
        imagelabel.cpp \
        labelControler.cpp \
        main.cpp \
        mainwindow.cpp \
        prolib.cpp \
        sharpeningOption.cpp \
        statisticalChart.cpp \
        templateChart.cpp

HEADERS += \
        binaryImageBox.h \
        function.h \
        grayImageProcesser.h \
        grayImageStatiscian.h \
        grayimage.h \
        huffmanTreeChart.h \
        imageControler.h \
        imageTransshaper.h \
        imagelabel.h \
        imagelimitation.h \
        labelControler.h \
        mainwindow.h \
        prolib.h \
        sharpeningOption.h \
        statisticalChart.h \
        templateChart.h

FORMS += \
        binaryImageBox.ui \
        huffmanTreeChart.ui \
        imageTransshaper.ui \
        mainwindow.ui \
        sharpeningOption.ui \
        statisticalChart.ui \
        templateChart.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
