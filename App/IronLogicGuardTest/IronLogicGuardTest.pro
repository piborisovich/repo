TEMPLATE = app
TARGET = IronLogicGuardTest

include($$PWD/../../template.pri)
include($$PWD/src/src.pri)
include($$PWD/deps.pri)

QT -= gui widgets

CONFIG += console c++11
