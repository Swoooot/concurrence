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
		for(int y=73;y<AREA_WIDTH;y++){
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


void movePerson(gridElement (*grid)[AREA_LENGTH], direction dir, person *movingPerson){
	int deltaX=0;
	int deltaY=0;

	//On determine le delta sur les coordonnes selon la direction
	if(dir==NORTHWEST){
		deltaX=-1;
		deltaY=-1;
	}
	else if(dir==NORTH){
		deltaY=-1;
	}
	else if(dir==WEST){
		deltaX=-1;
	}
	else if(dir==SOUTHWEST){
		deltaX=-1;
		deltaY=1;
	}
	else if(dir==SOUTH){
		deltaY=1;
	}

	//On deplace la personne
	movingPerson->x+=deltaX;
	movingPerson->y+=deltaY;

	if(deltaY!=0){
		for(int x=0;x<DELTA;x++){
			grid[movingPerson->y][movingPerson->x+x].p=*movingPerson;
			grid[movingPerson->y][movingPerson->x+x].st=OCCUPIED;

			grid[movingPerson->y+DELTA][movingPerson->x+x].st=FREE;
		}
	}
	if(deltaX!=0){

		for(int y=0;y<DELTA;y++){

			grid[movingPerson->y+y][movingPerson->x].p=*movingPerson;
			grid[movingPerson->y+y][movingPerson->x].st=OCCUPIED;

			grid[movingPerson->y+y][movingPerson->x+DELTA].st=FREE;

		}
	}


	//Les autres directions ne sont pas utilisees dans ce programme
}




int main(void){
	gridElement grid[AREA_WIDTH][AREA_LENGTH];
	initializeGrid(grid);
	amountOfPeople=15;
	fillGrid(grid);

	/*person testP;
	testP.x=400;
	testP.y=100;
	testP.id=30;
	*/
	addPerson(grid,400,100,30);
	//grid[100][400].st=OCCUPIED;
	//grid[100][400].p=testP;
	for(int y=0;y<AREA_WIDTH;y+=4){
	//	printf("%d: ",y);
		for(int x=0;x<AREA_LENGTH;x+=4){
			printf("%d|",grid[y][x].st);
		}
		printf("\n");
	}
	printf("x: %d, y: %d st:%d, id:%d\n",403,100,grid[100][403].st,grid[100][403].p.id);
	printf("x: %d, y: %d st:%d, id:%d\n",400,100,grid[100][402].st,grid[100][402].p.id);
	printf("x: %d, y: %d st:%d, id:%d\n",400,100,grid[100][401].st,grid[100][401].p.id);	
	printf("x: %d, y: %d st:%d, id:%d\n",400,100,grid[100][400].st,grid[100][400].p.id);
	printf("x: %d, y: %d st:%d\n",399,100,grid[100][399].st);
	movePerson(grid,WEST,&grid[100][400].p);
	printf("\n\n");
	printf("x: %d, y: %d st:%d\n",403,100,grid[100][403].st);
	printf("x: %d, y: %d st:%d, id:%d\n",403,100,grid[100][402].st,grid[100][403].p.id);
	printf("x: %d, y: %d st:%d, id:%d\n",403,100,grid[100][401].st,grid[100][402].p.id);
	printf("x: %d, y: %d st:%d, id:%d\n",400,100,grid[100][400].st,grid[100][401].p.id);
	printf("x: %d, y: %d st:%d, id:%d\n",399,100,grid[100][399].st,grid[100][400].p.id);

	/*for(int x=0;x<AREA_LENGTH;x++){
		for(int y=0;y<AREA_WIDTH;y++){
			printf("x:%d, y:%d, status:%d\n",x,y,grid[y][x].st);
		}
	}
	*/
	
	return 0;
}