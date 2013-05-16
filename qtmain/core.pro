TEMPLATE = lib

CONFIG -= qt
CONFIG += staticlib
#CONFIG += debug

SOURCES += \
    ../core/ppu.cpp \
    ../core/memory.cpp \
    ../core/cpu.cpp \
    ../core/nes.cpp \
    ../core/rom.cpp \
    ../core/memmap.cpp \
    ../core/controller.cpp \
    ../core/emuthread.cpp \
    ../core/threadutil.cpp \
    ../core/log.cpp \
    ../core/debug.cpp \
    ../core/test.cpp

HEADERS += \
    ../core/ppu.h \
    ../core/memory.h \
    ../core/cpu.h \
    ../core/nes.h \
    ../core/rom.h \
    ../global.h \
    ../core/memmap.h \
    ../core/controller.h \
    ../core/emuthread.h \
    ../core/threadutil.h \
    ../core/log.h \
    ../core/debug.h \
    ../core/test.h

QMAKE_CXXFLAGS += -pg
QMAKE_CFLAGS_DEBUG += -pg
