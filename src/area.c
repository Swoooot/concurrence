#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "../headers/area.h"


#define AREA_LENGTH 512
#define AREA_WIDTH 128
#define DELTA 4

int amountOfPeople;
person *allp;

//On initialise la grille avec les murs
void initializeGrid(gridElement (*grid)[AREA_LENGTH]){
	for(int x=0;x<AREA_LENGTH;x++){
		for(int y=0;y<AREA_WIDTH;y++){
			grid[y][x].st=FREE;
		}
	}
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
	/*for(int x=160;x<AREA_LENGTH;x++){
		grid[AREA_WIDTH-1][x].st=FREE;
	}
	*/
}

void addPerson(gridElement (*grid)[AREA_LENGTH], int x ,int y, int id){
	person p;
	p.id=id;
	p.x=x;
	p.y=y;
	allp[id]=p;
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
		bool done=false;
		while(done==false){
			bool ok=true;
			for(int j=0;j<4;j++){
				for(int k=0;k<4;k++){
					if(grid[newY+j][newX+k].st==OCCUPIED){
						srand(time(NULL));
						newX=rand()%(AREA_LENGTH-DELTA);
						newY=rand()%(AREA_WIDTH-DELTA);
						//Refaire un rand
						ok=false;
					}
				}
			}
			if(ok==true) done=true;
		}
		//Si on refait pas de rand on ajoute la personne a la grille
		addPerson(grid,newX,newY,addedId);
	//	printf("x: %d\n",newX);
	//	printf("y: %d\n",newY);
		//grid[newY][newX].st=OCCUPIED;
		
		//grid[newY][newX].p=p;
		
		addedId++;
	}

}

void shortestDistant(gridElement (*grid)[AREA_LENGTH], person p, int goalX, int goalY, direction *tableauDirection){
	double tableauDistance[5];
	//direction tableauDirection[5];
	double distanceNord = sqrt((((p.x)-goalX)*((p.x)-goalX))+(((p.y-1)-goalY)*((p.y-1)-goalY)));
	double distanceSud = sqrt((((p.x)-goalX)*((p.x)-goalX))+(((p.y+1)-goalY)*((p.y+1)-goalY)));
	double distanceOuest = sqrt((((p.x-1)-goalX)*((p.x-1)-goalX))+(((p.y)-goalY)*((p.y)-goalY)));
	double distanceNordOuest = sqrt((((p.x-1)-goalX)*((p.x-1)-goalX))+(((p.y-1)-goalY)*((p.y-1)-goalY)));
	double distanceSudOuest = sqrt((((p.x-1)-goalX)*((p.x-1)-goalX))+(((p.y+1)-goalY)*((p.y+1)-goalY)));

	
	tableauDistance[0] = distanceNord;
	tableauDirection[0] = NORTH;
	tableauDistance[1] = distanceSud;
	tableauDirection[1] = SOUTH;
	tableauDistance[2] = distanceOuest;
	tableauDirection[2] = WEST;
	tableauDistance[3] = distanceNordOuest;
	tableauDirection[3] = NORTHWEST;
	tableauDistance[4] = distanceSudOuest;
	tableauDirection[4] = SOUTHWEST;
	
	int i, j;
   for (i =1; i < 5; ++i) {
       double elem = tableauDistance[i];
       direction element = tableauDirection[i];
       for (j = i; j > 0 && tableauDistance[j-1] > elem; j--){
           	tableauDistance[j] = tableauDistance[j-1];
       		tableauDirection[j] = tableauDirection[j-1];}

       tableauDistance[j] = elem;
       tableauDirection[j] = element;
   }
	
	if(p.x==16 || p.x==160){
		printf("Position 16, id: %d\n",p.id);
		for(int k=0;k<5;k++){
			printf("Classement: %d, Direction: %d, distance: %f\n",k,tableauDirection[k],tableauDistance[k]);
		}
	}
	
	
	//return(tableauDirection);
	
	//On calcule la distance pour chaque direction et met dans un tableau trie

}

bool canMove(gridElement (*grid)[AREA_LENGTH], int x, int y, int deltaX, int deltaY){
	int newX=x+deltaX;
	int newY=y+deltaY;

	if(deltaY==-1){
		for(int dx=0;dx<DELTA;dx++){
			if(grid[newY][newX+dx].st!=FREE){
				//printf("x: %d, y: %d\n",newX+dx,newY);
				return false;
			}
		}
	}
	else if(deltaY==1){
		for(int dx=0;dx<DELTA;dx++){
			if(grid[y+DELTA][newX+dx].st!=FREE){
			//	printf("x: %d, y: %d\n",newX+dx,newY);
				return false;
			}
		}
	}
	if(deltaX!=0){
		for(int dy=0;dy<DELTA;dy++){
			if(grid[newY+dy][newX].st!=FREE){
				if(newX<4){
					printf("x: %d, y: %d, st:%d \n",newX,newY+dy,grid[newY+dy][newX].st);
				}
				return false;
			}
		}
	}
	return true;
}

void removePerson(gridElement (*grid)[AREA_LENGTH], person *p){
	for(int x=p->x;x<DELTA;x++){
		for(int y=p->y;y<p->y+DELTA;y++){
			//printf("id: %d, x: %d, y: %d\n",p->id,x,y);
			grid[y][x].st=FREE;
		}
	}
	p->id=-1;
}

bool movePerson(gridElement (*grid)[AREA_LENGTH], direction dir, person *movingPerson){
	int deltaX=0;
	int deltaY=0;

	//On determine le delta sur les coordonnes selon la direction
	if(dir==NORTHWEST){
		deltaX=-1;
		deltaY=(-1);
	}
	else if(dir==NORTH){
		deltaY=-1;
	}
	else if(dir==WEST){
		deltaX=-1;
	}
	else if(dir==SOUTHWEST){
		deltaX=(-1);
		deltaY=1;
	}
	else if(dir==SOUTH){
		deltaY=1;
	}
	

	if(movingPerson->x==16) printf("Dir: %d\n",dir);
	if(canMove(grid,movingPerson->x,movingPerson->y,deltaX,deltaY)==false){
		//printf("id: %d, dir: %d, x: %d, y: %d\n",movingPerson->id,dir,movingPerson->x,movingPerson->y);
		return false;
	}
	
	int newX=movingPerson->x+deltaX;
	int newY=movingPerson->y+deltaY;

	if(deltaY==-1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].p=*movingPerson;
			grid[newY][newX+x].st=OCCUPIED;

			grid[newY+DELTA][newX+x].st=FREE;
		}
	}
	else if(deltaY==1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].p=*movingPerson;
			grid[newY][newX+x].st=OCCUPIED;

			grid[movingPerson->y][newX+x].st=FREE;
		}
	}
	if(deltaX!=0){
		for(int y=0;y<DELTA;y++){
			grid[newY+y][newX].p=*movingPerson;
			grid[newY+y][newX].st=OCCUPIED;

			grid[newY+y][newX+DELTA].st=FREE;
			
		}

	}
	//On deplace la personne
	movingPerson->x+=deltaX;
	movingPerson->y+=deltaY;
	if(movingPerson->x==0) {
		removePerson(grid,movingPerson);
	}
	return true;


	//Les autres directions ne sont pas utilisees dans ce programme
}



void progress(gridElement (*grid)[AREA_LENGTH]){
	direction tableauDirection[5];
	int count=0;
	for(int a=0;a<5000;a++){
		for(int i=0;i<amountOfPeople;i++){
			if(allp[i].id!=-1){
				shortestDistant(grid,allp[i],0,AREA_WIDTH/2,tableauDirection);
				for(int j=0;j<5;j++){
					char move=movePerson(grid,tableauDirection[j],&allp[i]);
					if(move==true) break;
				}
				printf("direction: %d, id: %d, x: %d, y: %d\n",tableauDirection[0],allp[i].id,allp[i].x,allp[i].y);
			}
			else count++;
		}
		if(count==amountOfPeople){
			printf("Done\n");
			break;
		}
		else count=0;

		/*for(int i=0;i<amountOfPeople;i++){
			printf("id: %d, x: %d, y: %d\n",allp[i].id,allp[i].x,allp[i].y);
		}
		*/
		printf("\n");

		/*
		for(int y=0;y<AREA_WIDTH;y+=4){
			for(int x=0;x<AREA_LENGTH;x+=4){
				printf("%d|",grid[y][x].st);
			}
			printf("\n");
		}
		printf("\n");
		*/
	
	}
}




int main(void){
	gridElement grid[AREA_WIDTH][AREA_LENGTH];
	initializeGrid(grid);
	amountOfPeople=15;
	allp=malloc(sizeof(person)*amountOfPeople);
	fillGrid(grid);

	progress(grid);
	/*person testP;
	testP.x=400;
	testP.y=100;
	testP.id=30;
	*/
	//grid[100][400].st=OCCUPIED;
	//grid[100][400].p=testP;
	/*for(int y=0;y<AREA_WIDTH;y+=4){
	//	printf("%d: ",y);
		for(int x=0;x<AREA_LENGTH;x+=4){
			printf("%d|",grid[y][x].st);
		}
		printf("\n");
	}
	*/

	/*for(int x=0;x<AREA_LENGTH;x++){
		for(int y=0;y<AREA_WIDTH;y++){
			printf("x:%d, y:%d, status:%d\n",x,y,grid[y][x].st);
		}
	}
	*/
	
	return 0;
}