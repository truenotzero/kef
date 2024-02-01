#!/bin/bash

# Generic
SRC_DIR="src/"
MAIN="main"
DYNAMIC="dynamic"
FILE_ENDING=".c"
OLD=".old"

# Platform specific
EXE=".exe"
DLL=".dll"


if [ "$SRC_DIR$MAIN$FILE_ENDING" -nt "$MAIN$EXE" ]; then
    echo Recompiling "$SRC_DIR$MAIN$FILE_ENDING"
    clang "$SRC_DIR$MAIN$FILE_ENDING" -o "$MAIN$EXE"
fi
if [ "$SRC_DIR$DYNAMIC$FILE_ENDING" -nt "$DYNAMIC$DLL" ]; then
    echo Recompiling "$SRC_DIR$DYNAMIC$FILE_ENDING"
    mv -f "$DYNAMIC$DLL" "$DYNAMIC$DLL$OLD"
    clang "$SRC_DIR$DYNAMIC$FILE_ENDING" -o "$DYNAMIC$DLL" -shared
fi
