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
	//Ajout des murs
	for(int x=0;x<16;x++){
		for(int y=0;y<=60;y++){
			grid[y][x].st=OCCUPIED;
		}
		for(int y=69;y<AREA_WIDTH;y++){
			grid[y][x].st=OCCUPIED;
		}
	}
	for(int x=144;x<160;x++){
		for(int y=0;y<=56;y++){
			grid[y][x].st=OCCUPIED;
		}
		for(int y=72;y<AREA_WIDTH;y++){
			grid[y][x].st=OCCUPIED;
		}
	}
	for(int x=160;x<AREA_LENGTH;x++){
		grid[AREA_WIDTH-1][x].st=FREE;
	}
}

void addPerson(gridElement (*grid)[AREA_LENGTH], int x ,int y, int id){
	person p;
	p.id=id;
	p.x=x;
	p.y=y;
	//Faut l'ajotuer au 4*4
	for(int j=0;j<4;j++){
		for(int k=0;k<4;k++){
			grid[y+j][x+k].st=OCCUPIED;
			grid[y+j][x+k].p=p;
		}
	}
}

void fillGrid(gridElement (*grid)[AREA_LENGTH]){
	int addedId=0;

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
		addPerson(grid,newX,newY,addedId);
		printf("x: %d\n",newX);
		printf("y: %d\n",newY);
		//grid[newY][newX].st=OCCUPIED;
		
		//grid[newY][newX].p=p;

		addedId++;
	}

}

direction *shortestDistant(gridElement (*grid)[AREA_LENGTH], person p, int goalX, int goalY){
	//On calcule la distance pour chaque direction et met dans un tableau trie

}


void movePerson(gridElement (*grid)[AREA_LENGTH], direction dir, person movingPerson){
	if(dir==NORTH){
		movingPerson.y--;
		for(int x=0;x<DELTA;x++){
			grid[movingPerson.x+x][movingPerson.y].p=movingPerson;
			grid[movingPerson.x+x][movingPerson.y].st=OCCUPIED;

		//	grid[movingPerson.x+x][movingPerson.y+DELTA].p=NULL;
			grid[movingPerson.x+x][movingPerson.y+DELTA].st=FREE;
		}
	}
	if(dir==NORTHWEST){
		movingPerson.x--;
		movingPerson.y--;
		for(int x=0;x<DELTA;x++){
			grid[movingPerson.x+x][movingPerson.y].p=movingPerson;
			grid[movingPerson.x+x][movingPerson.y].st=OCCUPIED;

		//	grid[movingPerson.x+x][movingPerson.y+DELTA].p=NULL;
			grid[movingPerson.x+x][movingPerson.y+DELTA].st=FREE;
		}
		for(int y=0;y<DELTA;y++){
			grid[movingPerson.x][movingPerson.y+y].p=movingPerson;
			grid[movingPerson.x][movingPerson.y+y].st=OCCUPIED;

		//	grid[movingPerson.x+DELTA][movingPerson.y+y].p=NULL;
			grid[movingPerson.x+DELTA][movingPerson.y+y].st=FREE;
		}
	}
	if(dir==WEST){
		movingPerson.x--;
		for(int y=0;y<DELTA;y++){
			grid[movingPerson.x][movingPerson.y+y].p=movingPerson;
			grid[movingPerson.x][movingPerson.y+y].st=OCCUPIED;

		//	grid[movingPerson.x+DELTA][movingPerson.y+y].p=NULL;
			grid[movingPerson.x+DELTA][movingPerson.y+y].st=FREE;
		}
	}
	else if(dir==SOUTHWEST){
		movingPerson.x--;
		movingPerson.y++;
		for(int x=0;x<DELTA;x++){
			grid[movingPerson.x+x][movingPerson.y+DELTA].p=movingPerson;
			grid[movingPerson.x+x][movingPerson.y+DELTA].st=OCCUPIED;

		//	grid[movingPerson.x+x][movingPerson.y].p=NULL;
			grid[movingPerson.x+x][movingPerson.y].st=FREE;
		}
		for(int y=0;y<DELTA;y++){
			grid[movingPerson.x][movingPerson.y+y].p=movingPerson;
			grid[movingPerson.x][movingPerson.y+y].st=OCCUPIED;

		//	grid[movingPerson.x+DELTA][movingPerson.y+y].p=NULL;
			grid[movingPerson.x+DELTA][movingPerson.y+y].st=FREE;
		}

	}
	else if(dir==SOUTH){
		movingPerson.y++;
		for(int x=0;x<DELTA;x++){
			grid[movingPerson.x+x][movingPerson.y+DELTA].p=movingPerson;
			grid[movingPerson.x+x][movingPerson.y+DELTA].st=OCCUPIED;

		//	grid[movingPerson.x+x][movingPerson.y].p=NULL;
			grid[movingPerson.x+x][movingPerson.y].st=FREE;
		}
	}
	//Les autres directions ne sont pas utilisees dans ce programme
}




int main(void){
	gridElement grid[AREA_WIDTH][AREA_LENGTH];
	initializeGrid(grid);
	amountOfPeople=15;
	fillGrid(grid);

	for(int y=0;y<AREA_WIDTH;y++){
		printf("%d: ",y);
		for(int x=0;x<AREA_LENGTH;x++){
			printf("%d|",grid[y][x].st);
		}
		printf("\n");
	}

	/*for(int x=0;x<AREA_LENGTH;x++){
		for(int y=0;y<AREA_WIDTH;y++){
			printf("x:%d, y:%d, status:%d\n",x,y,grid[y][x].st);
		}
	}
	*/
	
	return 0;
}