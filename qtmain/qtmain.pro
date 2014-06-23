TEMPLATE = app

QT          += opengl


HEADERS     = \
              window.h \
    widget.h \
    paletteview.h \
    dataview.h \
    debugdialog.h \
    patternview.h \
    spriteview.h \
    nametableview.h
SOURCES     = \
              main.cpp \
              widget.cpp \
              window.cpp \
    paletteview.cpp \
    dataview.cpp \
    debugdialog.cpp \
    patternview.cpp \
    spriteview.cpp \
    nametableview.cpp

# install
#LIBS += -L../core/ -lcore
#PRE_TARGETDEPS += ../core/libcore.a
#CONFIG += link_pkgconfig

#INCLUDEPATH += $$PWD/../core
#DEPENDPATH += $$PWD/../core

FORMS += \
    debugdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/ -lcore

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/release/core.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/libcore.a

