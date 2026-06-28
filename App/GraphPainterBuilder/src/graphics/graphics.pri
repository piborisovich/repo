INCLUDEPATH += $$PWD

include($$PWD/items/items.pri)

HEADERS += \
           $$PWD/graphicsview.hpp \
           $$PWD/graphicsscene.hpp \
           $$PWD/itempropertieswidget.hpp \
           $$PWD/pathpropertieswidget.hpp \
           $$PWD/propertiesdockwidget.hpp \
           $$PWD/linepropertieswidget.hpp
SOURCES += \
           $$PWD/graphicsview.cpp \
           $$PWD/graphicsscene.cpp \
           $$PWD/itempropertieswidget.cpp \
           $$PWD/pathpropertieswidget.cpp \
           $$PWD/propertiesdockwidget.cpp \
           $$PWD/linepropertieswidget.cpp

FORMS   +=


