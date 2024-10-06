#-------------------------------------------------
#
# Project created by QtCreator 2024-09-05T15:56:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageTool
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
        mainwindow.cpp \
    edit.cpp

HEADERS += \
        mainwindow.h \
    edit.h

FORMS += \
        mainwindow.ui \
    edit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += c++11

unix:{
    contains(QT_ARCH, arm){
        VERSION = $$system(arm-none-linux-gnueabi-gcc -dumpversion)
        message("unix arm32,GCC版本:"$$VERSION)

        # opencv
        INCLUDEPATH += $$PWD/opencv_lib/arm32/include/opencv
        INCLUDEPATH += $$PWD/opencv_lib/arm32/include
        LIBS += -L$$PWD/opencv_lib/arm32/lib -lopencv_dnn -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_videoio -lopencv_imgcodecs -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_core
        LIBS += -L$$PWD/opencv_lib/arm32/share/OpenCV/3rdparty/lib -lIlmImf -llibprotobuf -llibtiff -llibjpeg-turbo -llibwebp -llibpng  -llibjasper -lzlib -lquirc -ldl -lm -lpthread -lrt

    }else{
        message("unix x86")
        # opencv
        unix: CONFIG += link_pkgconfig
        unix: PKGCONFIG += /usr/local/lib/x86_64-linux-gnu/pkgconfig/opencv.pc

    }
}

RESOURCES += \
    src.qrc

DISTFILES += \
    icons/1.png
