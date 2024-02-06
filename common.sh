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
INCLUDE_DIRS="-Iapi -Iinclude -Isrc -Isrc/api"
LIBRARY_DIRS="-Llib"
DEPENDENCIES="-lglfw3dll -lglew32 -lopengl32"
DEBUG="-g -O0 -fno-inline"
DEFINES=-D_CRT_SECURE_NO_WARNINGS

# Platform specific
PLAT="win"
EXE=".exe"
DLL=".dll"
