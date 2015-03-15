TEMPLATE = subdirs

SUBDIRS += \
    core.pro \
    qtmain.pro \
    unittest.pro

CONFIG += ordered
qtmain.depends = core.pro
unittest.depends = core.pro


