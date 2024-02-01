#!/bin/bash

# Generic
SRC_DIR="src/"
CORE="core/"
MAIN="main"
DYNAMIC="dynamic"
HOTRELOAD="khotreload"
FILE_ENDING=".c"
OLD=".old"
WARN="-Wall -Wextra -Wpedantic"

# Platform specific
PLAT="win"
EXE=".exe"
DLL=".dll"


if [ "$SRC_DIR$MAIN$FILE_ENDING" -nt "$MAIN$EXE" ]; then
    echo Recompiling "$SRC_DIR$MAIN$FILE_ENDING"
    clang "$SRC_DIR$MAIN$FILE_ENDING" "$SRC_DIR$CORE$HOTRELOAD-$PLAT$FILE_ENDING" -o "$MAIN$EXE" $WARN
fi
if [ "$SRC_DIR$DYNAMIC$FILE_ENDING" -nt "$DYNAMIC$DLL" ]; then
    echo Recompiling "$SRC_DIR$DYNAMIC$FILE_ENDING"
    mv -f "$DYNAMIC$DLL" "$DYNAMIC$DLL$OLD"
    clang "$SRC_DIR$DYNAMIC$FILE_ENDING" -o "$DYNAMIC$DLL" -shared $WARN
fi
