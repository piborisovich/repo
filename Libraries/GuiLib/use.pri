INCLUDEPATH += $$PWD/include

contains( CONFIG, staticlib ) {
    LIBS += $$ROOT/../LIB/libGuiLib.a
} else {
    LIBS += -L$$ROOT/../LIB -lGuiLib
}


