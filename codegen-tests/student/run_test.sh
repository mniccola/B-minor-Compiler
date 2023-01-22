#!/bin/sh

./bminor -codegen $1 program.s
gcc -g library.c program.s -o program
./program
