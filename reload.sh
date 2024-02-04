#!/bin/bash

source "./common.sh"

set -x

# if [ "$SRC_DIR$DYNAMIC$FILE_ENDING" -nt "$DYNAMIC$DLL" ]; then
echo Recompiling dynamic lib
mv -f dynamic.dll{,.old}
mv -f dynamic.exp{,.old}
mv -f dynamic.lib{,.old}
mv -f dynamic.pdb{,.old}
mv -f dynamic.ilk{,.old}
clang $DEBUG "$SRC_DIR$DYNAMIC$FILE_ENDING" -o "$DYNAMIC$DLL" -shared $WARN $INCLUDE_DIRS $LIBRARY_DIRS $DEPENDENCIES $DEFINES
if [[ "$?" -eq 0 ]]; then
    ./stamp timestamp.bin
else
    echo Not updating timestamp due to build errors
fi
#fi
