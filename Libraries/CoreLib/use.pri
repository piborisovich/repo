INCLUDEPATH += $$PWD/include

contains( CONFIG, staticlib ) {
    LIBS += $$ROOT/../LIB/libCoreLib.a
} else {
    LIBS += -L$$ROOT/../LIB -lCoreLib
}


