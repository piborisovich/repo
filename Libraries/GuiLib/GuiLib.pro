TEMPLATE = lib
TARGET = GuiLib

DEFINES += BUILD_WIDGETS_LIB

include($$PWD/src/src.pri)
include($$PWD/../../template.pri)

include($$PWD/deps.pri)

QT += help

CONFIG += c++11

RESOURCES += \
    guilib.qrc
