TEMPLATE = lib

CONFIG -= qt
CONFIG += staticlib
#CONFIG += debug

SOURCES += \
    ../global.cpp \
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
    ../core/test.cpp \
    ../core/mapper/mapper.cpp \
    ../core/mapper/mapper0.cpp \
    ../core/mapper/mapper1.cpp \
    ../core/mapper/mapper2.cpp \
    ../core/mapper/mapper4.cpp \
    ../core/asm.cpp

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
    ../core/test.h \
    ../core/mapper/mapper.h \
    ../core/mapper/mapper0.h \
    ../core/mapper/mapperunit.h \
    ../core/mapper/mapper1.h \
    ../core/mapper/mapper2.h \
    ../core/mapper/mapper4.h \
    ../core/asm.h

