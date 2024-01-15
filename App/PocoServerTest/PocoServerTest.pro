TEMPLATE = app
TARGET = PocoServerTest

include($$PWD/../../template.pri)
include($$PWD/src/src.pri)
include($$PWD/deps.pri)

CONFIG += console
CONFIG -= app_bundle
