#!/usr/bin/env sh
gcc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main &&
    ./main
