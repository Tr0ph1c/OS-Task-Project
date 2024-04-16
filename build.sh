#! /usr/bin/bash

include="-I./dependencies/include/"
libs="dependencies/libs/*"
links="-lopengl32 -lglfw3"
files="main.cpp"
g++ $include $libs -Wall $files -o main $links