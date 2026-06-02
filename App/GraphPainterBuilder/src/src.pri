INCLUDEPATH += $$PWD

include($$PWD/interfaces/interfaces.pri)
include($$PWD/tools/tools.pri)

HEADERS += \
           $$PWD/commands.hpp \
           $$PWD/flowlayout.hpp \
           $$PWD/graphicsview.hpp \
           $$PWD/core.hpp \
           $$PWD/layerswidget.hpp \
           $$PWD/mainwindow.hpp \
           $$PWD/graphicsscene.hpp \
           $$PWD/strings.hpp
SOURCES += \
           $$PWD/commands.cpp \
           $$PWD/core.cpp \
           $$PWD/flowlayout.cpp \
           $$PWD/graphicsview.cpp \
           $$PWD/layerswidget.cpp \
           $$PWD/main.cpp \
           $$PWD/graphicsscene.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/strings.cpp
FORMS   += $$PWD/mainwindow.ui


