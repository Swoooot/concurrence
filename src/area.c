#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/area.h"


#define AREA_LENGTH 512
#define AREA_WIDTH 128
#define DELTA 4

int amountOfPeople;

//On initialise la grille avec les murs
void initializeGrid(gridElement (*grid)[AREA_LENGTH]){
	for(int x=0;x<16;x++){
		for(int y=0;y<=60;y++){
			grid[y][x].st=OCCUPIED;
		}
		for(int y=69;y<AREA_WIDTH;y++){
			grid[y][x].st=OCCUPIED;
		}
	}
}

void fillGrid(gridElement (*grid)[AREA_LENGTH]){
	int added=0;

	for(int i=0;i<amountOfPeople;i++){
		int newX=rand()%(AREA_LENGTH-DELTA);
		int newY=rand()%(AREA_WIDTH-DELTA);
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				if(grid[newY+j][newX+k].st==OCCUPIED){
					srand(time(NULL));
					newX=rand()%(AREA_LENGTH-DELTA);
					newY=rand()%(AREA_WIDTH-DELTA);
					//Refaire un rand
				}
			}
		}
		//Si on refait pas de rand on ajoute la personne a la grille
		printf("x: %d\n",newX);
		printf("y: %d\n",newY);
		grid[newY][newX].st=OCCUPIED;
		person p;
		p.id=added;
		p.x=newX;
		p.y=newY;
		//Faut l'ajotuer au 4*4
		grid[newY][newX].p=p;

		added++;
	}

}


int main(void){
	gridElement grid[AREA_WIDTH][AREA_LENGTH];
	initializeGrid(grid);
	amountOfPeople=1;
	fillGrid(grid);

	for(int x=0;x<AREA_LENGTH;x++){
		for(int y=0;y<AREA_WIDTH;y++){
			printf("x:%d, y:%d, status:%d\n",x,y,grid[y][x].st);
		}
	}
	
	return 0;
}