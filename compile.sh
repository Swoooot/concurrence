#!/bin/bash
g++ -g -ansi -std=c++11 -Wall -o Person.o -c src/Person.cpp
g++ -g -ansi -std=c++11 -Wall -o GridElement.o -c src/GridElement.cpp
g++ -g -ansi -std=c++11 -Wall -o Grid.o -c src/Grid.cpp
g++ -g -ansi -std=c++11 -Wall -o main.o -c src/main.cpp
mkdir bin
g++ -o bin/main.exe main.o Grid.o GridElement.o Person.o -lpthread -lm 
echo "Compilation terminée (s'il y a une erreur de mkdir, la compilation a tout de même réussi)"