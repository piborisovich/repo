TEMPLATE = app
TARGET = GraphPainterBuilder

include($$PWD/../../template.pri)
include($$PWD/src/src.pri)


CONFIG += c++17

RESOURCES += \
    resources.qrc
