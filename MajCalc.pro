QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Src/gamehistorywindow.cpp \
    Src/hancyan.cpp \
    Src/main.cpp \
    Src/mainwindow.cpp \
    Src/ranksystem.cpp \
    Src/resultaddwindow.cpp \
    Src/settingswindow.cpp \
    Src/statisticwindow.cpp

HEADERS += \
    Include/gamehistorywindow.h \
    Include/hancyan.h \
    Include/mainwindow.h \
    Include/ranksystem.h \
    Include/resultaddwindow.h \
    Include/settingswindow.h \
    Include/statisticwindow.h

FORMS += \
    UI/gamehistorywindow.ui \
    UI/mainwindow.ui \
    UI/resultaddwindow.ui \
    UI/settingswindow.ui \
    UI/statisticwindow.ui

INCLUDEPATH += Include/
RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc \

DISTFILES +=



