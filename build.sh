#!/bin/bash

# Generic
SRC_DIR="src/"
CORE="core/"
MAIN="main"
SOURCES="$MAIN,api/kwindow"
DYNAMIC="dynamic"
HOTRELOAD="hotreload"
FILE_ENDING=".c"
OLD=".old"
WARN="-Wall -Wextra -Wpedantic"
INCLUDE_DIRS="-Iapi -Iinclude"
LIBRARY_DIRS="-Llib"
DEPENDENCIES="-lglfw3dll -lglew32 -lopengl32"
DEBUG=-g -O0

# Platform specific
PLAT="win"
EXE=".exe"
DLL=".dll"

set -x

# if [ "$SRC_DIR$MAIN$FILE_ENDING" -nt "$MAIN$EXE" ]; then
    echo Recompiling "$SRC_DIR$MAIN$FILE_ENDING"
    clang $DEBUG src/{main,api/{dy/{kdycode,kdycode-win},render/kwindow}}.c -o "$MAIN$EXE" $WARN $INCLUDE_DIRS $LIBRARY_DIRS $DEPENDENCIES
# fi
# if [ "$SRC_DIR$DYNAMIC$FILE_ENDING" -nt "$DYNAMIC$DLL" ]; then
    echo Recompiling "$SRC_DIR$DYNAMIC$FILE_ENDING"
    mv -f "$DYNAMIC$DLL" "$DYNAMIC$DLL$OLD"
    clang $DEBUG "$SRC_DIR$DYNAMIC$FILE_ENDING" -o "$DYNAMIC$DLL" -shared $WARN $INCLUDE_DIRS $LIBRARY_DIRS $DEPENDENCIES
# fi
