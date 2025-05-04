INCLUDEPATH += $$PWD/include

contains( CONFIG, staticlib ) {
    LIBS += $$ROOT/../LIB/libHexReader.a
} else {
    LIBS += -L$$ROOT/../LIB -lHexReader
}


