#-------------------------------------------------
#
# Project created by QtCreator 2023-05-15T10:03:15
#
#-------------------------------------------------

QT       += core gui network mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smarthome
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


SOURCES += \
    error.cpp \
    eyeform.cpp \
    jiemian.cpp \
        main.cpp \
    warning.cpp \
        widget.cpp \
    loginform.cpp \
    modify_pass.cpp \
    creat_anc.cpp

HEADERS += \
    error.h \
    eyeform.h \
    jiemian.h \
    warning.h \
        widget.h \
    loginform.h \
    modify_pass.h \
    creat_anc.h

FORMS += \
    error.ui \
    eyeform.ui \
    jiemian.ui \
    warning.ui \
        widget.ui \
    loginform.ui \
    modify_pass.ui \
    creat_anc.ui

RESOURCES += \
    pho.qrc \
    pic/pic.qrc

DISTFILES += \
    ../photo/2..gif \
    ../photo/QQ截图20230515004737.jpg \
    ../photo/QQ截图20230515004746.jpg \
    ../photo/QQ截图20230516090749.jpg \
    ../photo/QQ截图20230516112233.jpg \
    ../photo/绿叶.jpg \
    ../photo/清理前 (1).jpg \
    ../photo/清理前 (2).jpg \
    ../photo/设置透明背景.jpg \
    ../photo/RGB.png \
    ../photo/报警,警报,报警灯,危险.png \
    ../photo/窗帘安装.png \
    ../photo/灯泡.png \
    ../photo/返回 (1).png \
    ../photo/监控.png \
    ../photo/空调.png \
    ../photo/立方体 立体 多面体 方块 (1).png \
    ../photo/亮度.png \
    ../photo/门锁.png \
    ../photo/提示2.png \
    ../photo/温度.png \
    ../photo/1.webp \
    ../photo/OIP-C (1).jpg \
    ../photo/OIP-C (1).jpg \
    ../photo/OIP-C (1).jpg
UI_DIR = ../build-smarthome-Desktop_Qt_6_4_2_MinGW_64_bit-Debug