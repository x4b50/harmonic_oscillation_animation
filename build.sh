#!/usr/bin/env sh
set -xe

CLFAGS="-Wall -Werror -std=c11 -pedantic"
# https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

cc $CLFAGS main.c -o main $LIBS

# assuming that you have raylib compiled for windows in the parrent directory
# https://www.reddit.com/r/raylib/comments/uwpha2/comment/i9tmmv8/?context=3
WINLIBS="-lraylib -lopengl32 -lgdi32 -lwinmm -I../raylib/src -L../raylib/src"
x86_64-w64-mingw32-gcc $CLFAGS main.c -o main.exe $WINLIBS
