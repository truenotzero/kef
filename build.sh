#!/bin/bash

source "./common.sh"

set -x

# if [ "$SRC_DIR$MAIN$FILE_ENDING" -nt "$MAIN$EXE" ]; then
echo Recompiling executable
clang $DEBUG src/{main,core/kutil,api/{dy/{kdycode,kdycode-win},render/{kwindow,kprogram,kmesh}}}.c -o "$MAIN$EXE" $WARN $INCLUDE_DIRS $LIBRARY_DIRS $DEPENDENCIES $DEFINES
# fi
