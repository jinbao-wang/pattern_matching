QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imgprocess.cpp \
    main.cpp \
    mainwindow.cpp \
    pattern_matching.cpp \
    visualize.cpp

HEADERS += \
    imgprocess.h \
    mainwindow.h \
    pattern_matching.h \
    tools.h \
    visualize.h

INCLUDEPATH += \
            /opt/homebrew/Cellar/opencv@3/3.4.16_7/include \
            /opt/homebrew/Cellar/opencv@3/3.4.16_7/include/opencv \
            /opt/homebrew/Cellar/opencv@3/3.4.16_7/include/opencv2 \

LIBS += -L/opt/homebrew/Cellar/opencv@3/3.4.16_7/lib \
        -lopencv_calib3d    \
        -lopencv_core    \
        -lopencv_dnn    \
        -lopencv_features2d    \
        -lopencv_flann    \
        -lopencv_highgui    \
        -lopencv_imgcodecs    \
        -lopencv_imgproc    \
        -lopencv_ml    \
        -lopencv_objdetect    \
        -lopencv_photo    \
        -lopencv_stitching    \
        -lopencv_videoio    \
        -lopencv_video \


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
