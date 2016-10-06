#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>


#include "../headers/area.h"


#define AREA_LENGTH 512
#define AREA_WIDTH 128
#define DELTA 4

int amountOfPeople;
person *allp;
gridElement grid[AREA_WIDTH][AREA_LENGTH];

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
}

//On ajoute une personne à la grille
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
	//Ajoute des personnes a la grille
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
						//Si on arrive ici, une des cases est occupée, on genere un nouveau x et y aleatoirement
						srand(time(NULL));
						newX=rand()%(AREA_LENGTH-DELTA);
						newY=rand()%(AREA_WIDTH-DELTA);
						ok=false;
					}
				}
			}
			if(ok==true) done=true;
		}
		//Si on refait pas de rand on ajoute la personne a la grille
		addPerson(grid,newX,newY,addedId);

		
		addedId++;
	}

}

void shortestDistant(gridElement (*grid)[AREA_LENGTH], person p, int goalX, int goalY, direction *tableauDirection){
	double tableauDistance[5];
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

	
	//On calcule la distance pour chaque direction et met dans un tableau trie

}

bool canMove(gridElement (*grid)[AREA_LENGTH], int x, int y, int deltaX, int deltaY){
	int newX=x+deltaX;
	int newY=y+deltaY;
	//On vérifie si les cases vers lesquelles on veut se déplacer sont FREE
	if(deltaY==-1){
		for(int dx=0;dx<DELTA;dx++){
			if(grid[newY][newX+dx].st!=FREE){
				return false;
			}
		}
	}
	else if(deltaY==1){
		for(int dx=0;dx<DELTA;dx++){
			if(grid[y+DELTA][newX+dx].st!=FREE){
				return false;
			}
		}
	}
	if(deltaX!=0){
		for(int dy=0;dy<DELTA;dy++){
			if(grid[newY+dy][newX].st!=FREE){
				return false;
			}
		}
	}
	return true;
}

void removePerson(gridElement (*grid)[AREA_LENGTH], person *p){
	for(int x=p->x;x<DELTA;x++){
		for(int y=p->y;y<p->y+DELTA;y++){
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
	
	//if(movingPerson->x==16) printf("Dir: %d\n",dir);
	if(canMove(grid,movingPerson->x,movingPerson->y,deltaX,deltaY)==false){
		return false;
	}

	
	int newX=movingPerson->x+deltaX;
	int newY=movingPerson->y+deltaY;

	//Déplacement vertical
	if(deltaY==-1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].p=*movingPerson;
			grid[newY][newX+x].st=OCCUPIED;

			grid[newY+DELTA][movingPerson->x+x].st=FREE;
		}
	}
	else if(deltaY==1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].p=*movingPerson;
			grid[newY][newX+x].st=OCCUPIED;

			grid[movingPerson->y][movingPerson->x+x].st=FREE;
		}
	}
	//Déplacement horizental
	if(deltaX!=0){
		for(int y=0;y<DELTA;y++){
			grid[newY+y][newX].p=*movingPerson;
			grid[newY+y][newX].st=OCCUPIED;

			grid[movingPerson->y+y][newX+DELTA].st=FREE;
			
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


void *progressFour(void *arg){
	direction tableauDirection[5];
	int count=0;
	int threadId=(int) arg;
	int beginX, beginY = 0;
	int endX, endY = 0;

	//Intervalles de coordonnées gérées selon le thread
	if(threadId==0){
		beginX=0;
		beginY=0;
		endX=AREA_LENGTH/2;
		endY=AREA_WIDTH/2;
	}
	else if(threadId==1){
		beginX=AREA_LENGTH/2;
		beginY=0;
		endX=AREA_LENGTH;
		endY=AREA_WIDTH/2;
	}
	else if(threadId==2){
		beginX=0;
		beginY=AREA_WIDTH/2;
		endX=AREA_LENGTH/2;
		endY=AREA_WIDTH;
	}
	else if(threadId==3){
		beginX=AREA_LENGTH/2;
		beginY=AREA_WIDTH/2;
		endX=AREA_LENGTH;
		endY=AREA_WIDTH;
	}

	while(true){
		for(int i=0;i<amountOfPeople;i++){
			if(allp[i].id!=-1){
				person p=allp[i];
				if((p.x>=beginX && p.x<endX) && (p.y>=beginY && p.y<endY)){
					shortestDistant(grid,allp[i],0,AREA_WIDTH/2,tableauDirection);
					for(int j=0;j<5;j++){
					//	printf("Dir: %d\n",tableauDirection[j]);
						bool move=movePerson(grid,tableauDirection[j],&allp[i]);
						if(move==true) break;
					}
					//printf("thread: %d, direction: %d, id: %d, x: %d, y: %d\n",threadId,tableauDirection[0],allp[i].id,allp[i].x,allp[i].y);
				}
			}
			else {
				count++;
			}
		}
		if(count==amountOfPeople){
			printf("All done!\n");
			return NULL;
		}
		else count=0;
	}	


}



void fourThreads(gridElement (*grid)[AREA_LENGTH]){
	//x:256 y:64 millieu
	pthread_t pth0, pth1, pth2, pth3;

	clock_t t;
    t = clock();
    //Creation de 4 threads, on passe en paramètre un entier correspondant au numéro du thread
	pthread_create(&pth0,NULL,progressFour,(void *)0);
	pthread_create(&pth1,NULL,progressFour,(void *)1);
	pthread_create(&pth2,NULL,progressFour,(void *)2);
	pthread_create(&pth3,NULL,progressFour,(void *)3);

	//Attente de fin d'activité des 4 threads
	pthread_join(pth0, NULL);
	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	pthread_join(pth3, NULL);

	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    printf("Time required (4 threads): %f\n",time_taken);
	return;



}

void progress(gridElement (*grid)[AREA_LENGTH]){
	direction tableauDirection[5];
	int count=0;
	//Compteur temps
	clock_t t;
    t = clock();

    time_t time(time_t *tloc);

	while(true){
		for(int i=0;i<amountOfPeople;i++){
			if(allp[i].id!=-1){
				shortestDistant(grid,allp[i],0,AREA_WIDTH/2,tableauDirection);
				for(int j=0;j<5;j++){
					bool move=movePerson(grid,tableauDirection[j],&allp[i]);
					if(move==true) break;
				}
			//	printf("direction: %d, id: %d, x: %d, y: %d\n",tableauDirection[0],allp[i].id,allp[i].x,allp[i].y);
			}
			else count++;
		}
		if(count==amountOfPeople){
			printf("All done!\n");
			break;
		}
		else count=0;
		//printf("\n");

	
	}

	//Calculer temps passé pour effectuer la simulation
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time required (1 thread): %f\n",time_taken);
}


int main(int argc, char **argv){
	amountOfPeople=1;
	if (argc >= 1 ){

		for (int i=0; argv[i] != NULL; i++){
			if ( argv[i][1]=='p'){
				
				int puissance = argv[i][2] - 48;
				
				amountOfPeople = pow(2,puissance);
				printf("%d\n",amountOfPeople );
			}
			if (argv[i][1]=='t'){
				if (argv[i][2] == 0){

				}
				if (argv[i][2] == 1){

				} 
				if (argv[i][2] == 2){

				}

			}
			if (argv[i][1]=='m'){
				//executionTime()
			}

		}
		

	}
	printf("%d\n",amountOfPeople);
	initializeGrid(grid);
	//amountOfPeople=64;
	allp=malloc(sizeof(person)*amountOfPeople);
	fillGrid(grid);

	progress(grid);
	//fourThreads(grid);
	
	return 0;
}