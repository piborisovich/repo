INCLUDEPATH += $$PWD/include

contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/lib/x32 -lZGuard
} else {
        LIBS += -L$$PWD/lib/x64 -lZGuard
}


