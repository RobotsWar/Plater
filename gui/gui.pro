#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T10:59:43
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    viewer.cpp \
    wizard.cpp \
    platesviewer.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    worker.h \
    viewer.h \
    wizard.h \
    platesviewer.h \
    about.h

FORMS    += mainwindow.ui \
    wizard.ui \
    platesviewer.ui \
    about.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/release/ -llibplater
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/debug/ -llibplater
else:symbian: LIBS += -llibplater
else:unix: LIBS += -L$$PWD/../build/ -llibplater

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build/release/libplater.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build/debug/libplater.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../build/liblibplater.a

LIBS += -lGLU -lglut
