INCLUDEPATH += $$PWD

include($$PWD/items/items.pri)
include($$PWD/widgets/widgets.pri)
include($$PWD/effects/effects.pri)

HEADERS += \
           $$PWD/graphicsview.hpp \
           $$PWD/graphicsscene.hpp

SOURCES += \
           $$PWD/graphicsview.cpp \
           $$PWD/graphicsscene.cpp

FORMS   +=


