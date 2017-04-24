QT       += core gui dbus x11extras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app


TARGET = elokab-notify
DESTDIR = usr/bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
INCLUDEPATH +=build
LIBS += $(SUBLIBS)  -L../usr/lib -L$$PWD/../usr/lib
QMAKE_INCDIR += $$PWD/../library/include $$PWD/../library

HEADERS += \
    elokabnotify_adaptor.h \
    elokabnotification.h \
    notificationgui.h

SOURCES += main.cpp \
    elokabnotify_adaptor.cpp \
    elokabnotification.cpp \
    notificationgui.cpp



FORMS += \
    notificationgui.ui
#INSTALL
 target.path = /usr/bin
 INSTALLS +=    target

linux-g++*: {
             # Provide relative path from application to elokab librarys
             # ex:app=usr/bin/elokab-applications  library=usr/lib/libelokabmimicon.so
             QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
             }
