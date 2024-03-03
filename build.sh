#!/usr/bin/env sh
set -xe

CLFAGS="-Wall -Werror -std=c11 -pedantic"
# https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

cc $CLFAGS main.c -o main $LIBS

# https://www.reddit.com/r/raylib/comments/uwpha2/comment/i9tmmv8/?context=3
# x86_64-w64-mingw32-gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main.exe
