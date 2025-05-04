#!/bin/bash

RED_ESC="\033[0;31m"
GREEN_ESC="\033[0;32m"
BROWN_ESC="\033[0;33m"
BLUE_ESC="\033[0;34m"
MAGENTA_ESC="\033[0;35m"
CYAN_ESC="\033[0;36m"
WHITE_ESC="\033[0;37m"
GRAY_ESC="\033[0;90m"
END_ESC="\033[0m"

function Display() {
    COLOR=""
    PREAMBLE=""
    TEXT=""

    if [ $# == 2 ] || [ $# == 3 ]; then
        if [ $1 == "Red" ]; then
            COLOR=$RED_ESC
        elif [ $1 == "Green" ]; then
            COLOR=$GREEN_ESC
        elif [ $1 == "Brown" ]; then
            COLOR=$BROWN_ESC
        elif [ $1 == "Blue" ]; then
            COLOR=$BLUE_ESC
        elif [ $1 == "Magenta" ]; then
            COLOR=$MAGENTA_ESC
        elif [ $1 == "Cyan" ]; then
            COLOR=$CYAN_ESC
        elif [ $1 == "White" ]; then
            COLOR=$WHITE_ESC
        elif [ $1 == "Gray" ]; then
            COLOR=$GRAY_ESC
        fi
    fi

    if [ $# == 1 ]; then
        TEXT="$1"
    elif [ $# == 2 ]; then
        TEXT="$2"
    elif [ $# == 3 ]; then
        PREAMBLE="$2 "
        TEXT="$3"
    else
        return -1;
    fi

    echo -e "$PREAMBLE$COLOR$TEXT$END_ESC"

    return 0
}

function get_build_version() {

eval "$1=''"

describe=$(git describe)
arrDescribe=(${describe//-/ })
arrLen=${#arrDescribe[@]}

if (($arrLen > 1)); then
	eval "$1=${arrDescribe[0]:1}.${arrDescribe[1]}"
fi

return 0

}

function BackupLocalPri() {
    LOCAL_PATH="$ROOT_DIR/local.pri"
    if [ -a $LOCAL_PATH ]; then
        mv $LOCAL_PATH "${LOCAL_PATH}_backup"
        return 0
    fi

    return -1
}

function CreateLocalPri() {
    LOCAL_PATH="$ROOT_DIR/local.pri"
    touch "$LOCAL_PATH"

    echo "CONFIG+=staticlib" >> "$LOCAL_PATH"
    echo "QT-=gui" >> "$LOCAL_PATH"

    return 0
}

function RemoveLocalPri() {
    LOCAL_PATH="$ROOT_DIR/local.pri"
    if [ -a $LOCAL_PATH ]; then
        rm $LOCAL_PATH
        return 0
    fi

    return -1
}

function RestoreLocalPri() {
    LOCAL_PATH="$ROOT_DIR/local.pri"
    BACKUP_LOCAL_PATH="$ROOT_DIR/local.pri_backup"
    if [ -a $BACKUP_LOCAL_PATH ]; then
        mv $BACKUP_LOCAL_PATH "${LOCAL_PATH}"
        return 0
    fi

    return -1
}

Display Blue "=============================BUILD_SCRIPT============================"

BUILD_TYPE="release"

if [ "$1" == "debug" ]; then
    BUILD_TYPE="debug"
fi

APPLICATION_DIR=$(pwd)
TEST_PROJECT_NAME=HexReaderLib
TARGET_NAME=HexSegmentationTest

cd $(pwd)/../../../..
ROOT_DIR=$(pwd)
WORK_DIR=$ROOT_DIR/..

LIBS_TO_BUILD=(
"$ROOT_DIR/Lib/CoreLib"
"$ROOT_DIR/Lib/HexReader")


BIN_DIR=$WORK_DIR/BIN/tests/$TEST_PROJECT_NAME/$TARGET_NAME
LIB_DIR="$WORK_DIR/LIB"
BUILD_DIR="$WORK_DIR/build"

Display ""

Display Blue "=============================BUILD_INFO============================"

Display Brown "BUILD_TYPE" "$BUILD_TYPE"
Display Brown "ROOT_DIR" "$ROOT_DIR"
Display Brown "WORK_DIR" "$WORK_DIR"
Display Brown "BIN_DIR" "$BIN_DIR"
Display Brown "LIB_DIR" "$LIB_DIR"
Display Brown "BUILD_DIR" "$BUILD_DIR"

Display ""
Display "Libraries to build"

for path in "${LIBS_TO_BUILD[@]}"; do
    Display Brown "$path"
done

Display Blue "==================================================================="

#Remove BUILD_DIR
rm -rf "$BUILD_DIR"

#Remove BIN_DIR
rm -rf "$BIN_DIR"

#Create LIB_DIR
if [ ! -d "$LIB_DIR" ]; then
    mkdir "$LIB_DIR"
fi

cd "$LIB_DIR"

rm lib*

#Резервная копия local.pri
BackupLocalPri
#Создать временный local.pri
CreateLocalPri

Display Green "=========================BUILD_LIBRARIES_START====================="

for path in "${LIBS_TO_BUILD[@]}"; do
    cd "$path"
    Display Magenta "Build" "$path"
    /usr/bin/make clean -j8
    /usr/bin/qmake "CONFIG+=$BUILD_TYPE" && /usr/bin/make -j8

    #Command exit status
    if [ $? != 0 ]; then
        exit $?
    fi
done

Display Green "=======================BUILD_TEST_START===================="

cd "$APPLICATION_DIR"

get_build_version version

export BUILD_VERSION="$version"

Display Brown "BUILD_VERSION" "$BUILD_VERSION"

/usr/bin/make clean -j8
/usr/bin/qmake "CONFIG+=$BUILD_TYPE" && /usr/bin/make -j8

cp -R "$APPLICATION_DIR/resources/." "$BIN_DIR"

rm -rf "$BUILD_DIR"

#Удалить временный local.pri
RemoveLocalPri
#Восстановить прежнюю версию local.pri
RestoreLocalPri

