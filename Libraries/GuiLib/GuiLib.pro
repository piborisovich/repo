TEMPLATE = lib
TARGET = GuiLib

include($$PWD/src/src.pri)
include($$PWD/../../template.pri)

include($$PWD/deps.pri)

QT += help

CONFIG += c++11

RESOURCES += \
    guilib.qrc
