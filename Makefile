area: area.o
	gcc -o area area.o -lpthread -lm

area.o: src/area.c headers/area.h
	gcc -g -Wall -std=gnu99 -o area.o -c src/area.c