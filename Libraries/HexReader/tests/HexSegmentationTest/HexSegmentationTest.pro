
TARGET = HexSegmentationTest
TEST_PROJECT=HexReaderLib

include($$PWD/../../../../template.pri)
include($$PWD/deps.pri)
include($$PWD/src/src.pri)

QT -= gui widgets

QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DESTDIR = $$ROOT/../BIN/tests/$$TEST_PROJECT/$$TARGET

# must use a variable as input
TEST_DEPS = .
PreBuildEvent.input = TEST_DEPS
# use non-existing file here to execute every time
PreBuildEvent.output = out.txt

win32 {
    PRE_BUILD_FILE = $$PWD/prebuild.bat
    # the system call to the batch file
    PreBuildEvent.commands = call $$PRE_BUILD_FILE
}

unix {
    PRE_BUILD_FILE = $$PWD/prebuild.sh
    # the system call to the batch file
    PreBuildEvent.commands = /bin/bash $$PRE_BUILD_FILE
}

exists($$PRE_BUILD_FILE) {
    # some name that displays during execution
    PreBuildEvent.name = running Pre-Build steps...
    # "no_link" tells qmake we don’t need to add the output to the object files for linking, and "no_clean" means there is no clean step for them.
    # "target_predeps" tells qmake that the output of this needs to exist before we can do the rest of our compilation.
    PreBuildEvent.CONFIG += no_link no_clean target_predeps
    # Add the compiler to the list of 'extra compilers'.
    QMAKE_EXTRA_COMPILERS += PreBuildEvent
}
