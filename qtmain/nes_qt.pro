TEMPLATE = subdirs

SUBDIRS += \
    core.pro \
    qtmain.pro

CONFIG += ordered
qtmain.depends = core.pro

