#!/bin/bash

g++ -c e-task.cpp -o e-task.o
g++ -c tree_dump.cpp -o tree_dump.o
g++ e-task.cpp tree_dump.o -o aboba.out
./aboba.out