#!/bin/bash

echo "=============================PREBUILD============================"

APP_DIR=$(pwd)
ROOT_DIR=$APP_DIR/../../../..
WORK_DIR=$ROOT_DIR/..
LIB_DIR=$WORK_DIR/LIB
RESOURCES_DIR=$APP_DIR/resources
BIN_DIR=$WORK_DIR/BIN/tests/HexReaderLib/HexSegmentationTest

cd $RESOURCES_DIR
cp -R . $BIN_DIR
