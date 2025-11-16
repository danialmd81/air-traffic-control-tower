QT       += core
QT       += gui
QT       += location
QT       += quick quickwidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FlyingObjects/airplane.cpp \
    FlyingObjects/helicopter.cpp \
    FlyingObjects/jet.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FlyingObjects/airplane.h \
    FlyingObjects/helicopter.h \
    FlyingObjects/jet.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    main.qml

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../RadarDataReceiver/build/Desktop_Qt_6_9_3-Release/release/ -lRadarDataReceiver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../RadarDataReceiver/build/Desktop_Qt_6_9_3-Release/debug/ -lRadarDataReceiver
else:unix: LIBS += -L$$PWD/../RadarDataReceiver/build/Desktop_Qt_6_9_3-Release/ -lRadarDataReceiver

INCLUDEPATH += $$PWD/../RadarDataReceiver
DEPENDPATH += $$PWD/../RadarDataReceiver
