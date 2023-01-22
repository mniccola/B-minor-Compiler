#!/bin/sh

./bminor -codegen $1 program.s
gcc library.c program.s -o program
./program
