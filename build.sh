#!/usr/bin/env sh
# https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main
# https://www.reddit.com/r/raylib/comments/uwpha2/comment/i9tmmv8/?context=3
# x86_64-w64-mingw32-gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main.exe
