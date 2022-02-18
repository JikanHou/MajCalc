QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamehistorywindow.cpp \
    hancyan.cpp \
    main.cpp \
    mainwindow.cpp \
    ranksystem.cpp \
    resultaddwindow.cpp \
    settingswindow.cpp \
    statisticwindow.cpp

HEADERS += \
    gamehistorywindow.h \
    hancyan.h \
    mainwindow.h \
    ranksystem.h \
    resultaddwindow.h \
    settingswindow.h \
    statisticwindow.h

FORMS += \
    gamehistorywindow.ui \
    mainwindow.ui \
    resultaddwindow.ui \
    settingswindow.ui \
    statisticwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

DISTFILES +=



