INCLUDEPATH += $$PWD

include($$PWD/items/items.pri)

HEADERS += \
           $$PWD/constraintlineitem.hpp \
           $$PWD/graphicsview.hpp \
           $$PWD/graphicsscene.hpp \
           $$PWD/itempropertieswidget.hpp \
           $$PWD/propertiesdockwidget.hpp \
           $$PWD/linepropertieswidget.hpp
SOURCES += \
           $$PWD/constraintlineitem.cpp \
           $$PWD/graphicsview.cpp \
           $$PWD/graphicsscene.cpp \
           $$PWD/itempropertieswidget.cpp \
           $$PWD/propertiesdockwidget.cpp \
           $$PWD/linepropertieswidget.cpp

FORMS   +=


