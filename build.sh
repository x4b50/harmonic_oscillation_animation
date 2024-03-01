#!/usr/bin/env sh
# https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main &&
    ./main
