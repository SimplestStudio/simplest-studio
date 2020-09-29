QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    extracting.cpp \
    main.cpp \
    mainwindow.cpp \
    progress.cpp \
    settings.cpp \
    smartsettings.cpp \
    taskcomplete.cpp

HEADERS += \
    about.h \
    extracting.h \
    mainwindow.h \
    progress.h \
    settings.h \
    smartsettings.h \
    taskcomplete.h

FORMS += \
    about.ui \
    extracting.ui \
    mainwindow.ui \
    progress.ui \
    settings.ui \
    smartsettings.ui \
    taskcomplete.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    files.qrc

unix:!macx: LIBS += -lmediainfo
