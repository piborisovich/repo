INCLUDEPATH += $$PWD

include($$PWD/items/items.pri)

HEADERS += \
           $$PWD/blureffectwidget.hpp \
           $$PWD/circlepropertieswidget.hpp \
           $$PWD/itempropertieswidget.hpp \
           $$PWD/pathpropertieswidget.hpp \
           $$PWD/propertiesdockwidget.hpp \
           $$PWD/linepropertieswidget.hpp \
           $$PWD/rectpropertieswidget.hpp
SOURCES += \
           $$PWD/blureffectwidget.cpp \
           $$PWD/circlepropertieswidget.cpp \
           $$PWD/itempropertieswidget.cpp \
           $$PWD/pathpropertieswidget.cpp \
           $$PWD/propertiesdockwidget.cpp \
           $$PWD/linepropertieswidget.cpp \
           $$PWD/rectpropertieswidget.cpp

FORMS   += \
    $$PWD/blureffectwidget.ui


