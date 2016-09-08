
TEMPLATE = app
TARGET = AutoHideDockWidget
DESTDIR = ../Debug
QT += core widgets gui
CONFIG += debug
DEFINES += QT_LARGEFILE_SUPPORT QT_DLL
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

FORMS += \
    NXMainWindow.ui

HEADERS += \
    NXDockWidget.h \
    NXDockWidgetTabBar.h \
    NXDockWidgetTabButton.h \
    NXDockWidgetTitle.h \
    NXMainWindow.h

SOURCES += \
    main.cpp \
    NXDockWidget.cpp \
    NXDockWidgetTabBar.cpp \
    NXDockWidgetTabButton.cpp \
    NXDockWidgetTitle.cpp \
    NXMainWindow.cpp

RESOURCES += \
    nxmainwindow.qrc
