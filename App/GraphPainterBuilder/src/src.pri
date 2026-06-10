INCLUDEPATH += $$PWD

include($$PWD/interfaces/interfaces.pri)
include($$PWD/graphics/graphics.pri)
include($$PWD/tools/tools.pri)

HEADERS += \
           $$PWD/commands.hpp \
           $$PWD/flowlayout.hpp \
           $$PWD/core.hpp \
           $$PWD/layerswidget.hpp \
           $$PWD/mainwindow.hpp \
           $$PWD/strings.hpp
SOURCES += \
           $$PWD/commands.cpp \
           $$PWD/core.cpp \
           $$PWD/flowlayout.cpp \
           $$PWD/layerswidget.cpp \
           $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/strings.cpp
FORMS   += $$PWD/mainwindow.ui


