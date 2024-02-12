#!/bin/bash

source "./common.sh"

set -x

# if [ "$SRC_DIR$MAIN$FILE_ENDING" -nt "$MAIN$EXE" ]; then
echo Recompiling executable
clang $DEBUG src/{main,core/{kutil,stb_image,kmath},api/{dy/{kdycode,kdycode-win},render/{kwindow,kprogram,kmesh,ktexture,kcamera}}}.c -o "$MAIN$EXE" $WARN $INCLUDE_DIRS $LIBRARY_DIRS $DEPENDENCIES $DEFINES
# fi
