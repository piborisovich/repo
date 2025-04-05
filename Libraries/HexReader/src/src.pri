INCLUDEPATH += $$PWD

include($$PWD/interfaces/interfaces.pri)

HEADERS += $$PWD/ChecksumCheckerGenerator.hpp \
           $$PWD/HexReader.hpp \
           $$PWD/HexReadonlyDataset.hpp \
           $$PWD/HexRecord.hpp \
           $$PWD/HexToStringCodec.hpp \
           $$PWD/hextypes.hpp \
           $$PWD/hex_global.hpp

SOURCES += $$PWD/ChecksumCheckerGenerator.cpp \
           $$PWD/HexReader.cpp \
           $$PWD/HexRecord.cpp \
           $$PWD/HexToStringCodec.cpp
