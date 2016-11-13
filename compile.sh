#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
g++ -g -ansi -std=c++11 -Wall -o $DIR/Person.o -c $DIR/src/Person.cpp
g++ -g -ansi -std=c++11 -Wall -o $DIR/GridElement.o -c $DIR/src/GridElement.cpp
g++ -g -ansi -std=c++11 -Wall -o $DIR/Grid.o -c $DIR/src/Grid.cpp
g++ -g -ansi -std=c++11 -Wall -o $DIR/ThreadArg.o -c $DIR/src/ThreadArg.cpp
g++ -g -ansi -std=c++11 -Wall -o $DIR/main.o -c $DIR/src/main.cpp
mkdir -p $DIR/bin
g++ -o $DIR/bin/main.exe $DIR/main.o $DIR/Grid.o $DIR/GridElement.o $DIR/Person.o $DIR/ThreadArg.o -lpthread -lm 
echo "Compilation terminée "