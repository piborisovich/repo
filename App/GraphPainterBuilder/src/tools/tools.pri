INCLUDEPATH += $$PWD

include($$PWD/gui/gui.pri)
include($$PWD/settings/settings.pri)

HEADERS += \
    $$PWD/dragtool.hpp \
    $$PWD/graphtool.hpp \
    $$PWD/pipettetool.hpp \
    $$PWD/selecttool.hpp \
    $$PWD/tool.hpp \
    $$PWD/brushtool.hpp \
    $$PWD/circletool.hpp \
    $$PWD/toolcreator.hpp \
    $$PWD/erasertool.hpp \
    $$PWD/linetool.hpp \
    $$PWD/toollist.hpp \
    $$PWD/rectangletool.hpp

SOURCES += \
    $$PWD/dragtool.cpp \
    $$PWD/graphtool.cpp \
    $$PWD/pipettetool.cpp \
    $$PWD/selecttool.cpp \
    $$PWD/tool.cpp \
    $$PWD/brushtool.cpp \
    $$PWD/circletool.cpp \
    $$PWD/toolcreator.cpp \
    $$PWD/erasertool.cpp \
    $$PWD/linetool.cpp \
    $$PWD/rectangletool.cpp

FORMS   +=

