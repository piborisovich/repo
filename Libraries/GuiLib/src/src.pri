INCLUDEPATH += $$PWD

include($$PWD/models/models.pri)

HEADERS += \
    $$PWD/messagebox.hpp \
    $$PWD/helpwidget.hpp \
    $$PWD/helptextbrowser.hpp \
    $$PWD/gui_global.hpp \
    $$PWD/strings.hpp
	
SOURCES += \
    $$PWD/messagebox.cpp \
    $$PWD/helpwidget.cpp \
    $$PWD/helptextbrowser.cpp \
    $$PWD/strings.cpp
	
FORMS   += $$PWD/helpwidget.ui 
           



