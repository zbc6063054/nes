TEMPLATE = app

SOURCES += \
    ../unittest/testmain.cpp \
    ../unittest/mappertest.cpp


# install
#LIBS += -L../core/ -lcore
#PRE_TARGETDEPS += ../core/libcore.a
#CONFIG += link_pkgconfig

#INCLUDEPATH += $$PWD/../core
#DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/ -lcore

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/release/core.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/libcore.a

unix:!macx: LIBS += -L$$PWD/../unittest/gtest/lib/ -lgtest

INCLUDEPATH += $$PWD/../unittest/gtest/include
DEPENDPATH += $$PWD/../unittest/gtest/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../unittest/gtest/lib/libgtest.a
