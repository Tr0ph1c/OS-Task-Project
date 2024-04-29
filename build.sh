#! /usr/bin/bash

include="-I./dependencies/include/"
libs="./dependencies/libs/*.o"
links="-lglfw -lGL"
files="main.cpp"
g++ $include $libs -Wall $files -o main $links -g
