INCLUDEPATH += $$PWD

include($$PWD/interfaces/interfaces.pri)

contains( QT, gui ) {
    include($$PWD/gui/gui.pri)
    include($$PWD/models/models.pri)
}

HEADERS += $$PWD/hexreader.hpp \
    $$PWD/checksumcheckergenerator.hpp \
    $$PWD/hex_global.hpp \
    $$PWD/hexrecord.hpp \
    $$PWD/hextostringcodec.hpp \
    $$PWD/hexreadonlydataset.hpp\
    $$PWD/hextypes.hpp
SOURCES += $$PWD/hexreader.cpp \
    $$PWD/checksumcheckergenerator.cpp \
    $$PWD/hexrecord.cpp \
    $$PWD/hextostringcodec.cpp



