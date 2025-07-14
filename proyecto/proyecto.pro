QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animaciones.cpp \
    goku.cpp \
    kameha.cpp \
    main.cpp \
    mainwindow.cpp \
    mensajefinal.cpp \
    menu.cpp \
    nivel1.cpp \
    nivel2.cpp \
    nivelbase.cpp \
    obstaculo.cpp \
    personaje.cpp \
    robot.cpp \
    robotai.cpp

HEADERS += \
    animaciones.h \
    goku.h \
    kameha.h \
    mainwindow.h \
    mensajefinal.h \
    menu.h \
    nivel1.h \
    nivel2.h \
    nivelbase.h \
    obstaculo.h \
    personaje.h \
    robot.h \
    robotai.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc
