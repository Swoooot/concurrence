//AKHMADOV Baisangour
//LEVY Cyprien

#include <iostream>
#include "Grid.h"
#include "Person.h"
#include "GridElement.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


using namespace std;


Grid::Grid()
{
    this->grid.resize(AREA_WIDTH);
	for(int i=0;i<AREA_WIDTH;i++){
        this->grid[i].resize(AREA_LENGTH);
	}
	//Ajout des murs
	for(int x=0;x<16;x++){
		for(int y=0;y<=60;y++){
		    this->grid[y][x].setStatus(OCCUPIED);
		}
		for(int y=69;y<AREA_WIDTH;y++){
		    this->grid[y][x].setStatus(OCCUPIED);
		}
	}
	for(int x=144;x<160;x++){
		for(int y=0;y<=56;y++){
		    this->grid[y][x].setStatus(OCCUPIED);
		}
		for(int y=73;y<AREA_WIDTH;y++){
		    this->grid[y][x].setStatus(OCCUPIED);
		}
	}
	this->amountOfPeople=0;

}

Grid::~Grid()
{
}

//On ajoute une personne à la grille
void Grid::addPerson(int x ,int y, int id){
	Person p;
	p.setId(id);
	p.setX(x);
	p.setY(y);
	this->allp.push_back(p);
	//Faut l'ajotuer au 4*4
	for(int j=0;j<4;j++){
		for(int k=0;k<4;k++){
			this->grid[y+j][x+k].setStatus(OCCUPIED);
			this->grid[y+j][x+k].setPerson(p);
		}
	}
}


void Grid::fillGrid(int amountOfPeople){
	//Ajoute des personnes a la grille
	this->amountOfPeople=amountOfPeople;
	int addedId=0;

	for(int i=0;i<this->amountOfPeople;i++){
		int newX=rand()%(AREA_LENGTH-DELTA);
		int newY=rand()%(AREA_WIDTH-DELTA);
		bool done=false;
		while(done==false){
			bool ok=true;
			for(int j=0;j<4;j++){
				for(int k=0;k<4;k++){
					if(this->grid[newY+j][newX+k].getStatus()==OCCUPIED){
						//Si on arrive ici, une des cases est occupée, on genere un nouveau x et y aleatoirement
					//	srand(time(NULL));
						newX=rand()%(AREA_LENGTH-DELTA);
						newY=rand()%(AREA_WIDTH-DELTA);
						ok=false;
					}
				}
			}
			if(ok==true) done=true;
		}
		//Si on refait pas de rand on ajoute la personne a la grille
		addPerson(newX,newY,addedId);


		addedId++;
	}

}

void Grid::shortestDistant(Person p, int goalX, int goalY, vector<Direction> &tableauDirection){

	vector<double> tableauDistance(5);

	//TODO getter pour p au lieu d'acces direct
	double distanceNord = sqrt((((p.getX())-goalX)*((p.getX())-goalX))+(((p.getY()-1)-goalY)*((p.getY()-1)-goalY)));
	double distanceSud = sqrt((((p.getX())-goalX)*((p.getX())-goalX))+(((p.getY()+1)-goalY)*((p.getY()+1)-goalY)));
	double distanceOuest = sqrt((((p.getX()-1)-goalX)*((p.getX()-1)-goalX))+(((p.getY())-goalY)*((p.getY())-goalY)));
	double distanceNordOuest = sqrt((((p.getX()-1)-goalX)*((p.getX()-1)-goalX))+(((p.getY()-1)-goalY)*((p.getY()-1)-goalY)));
	double distanceSudOuest = sqrt((((p.getX()-1)-goalX)*((p.getX()-1)-goalX))+(((p.getY()+1)-goalY)*((p.getY()+1)-goalY)));


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
        Direction element = tableauDirection[i];
        for (j = i; j > 0 && tableauDistance[j-1] > elem; j--){
            tableauDistance[j] = tableauDistance[j-1];
            tableauDirection[j] = tableauDirection[j-1];}

        tableauDistance[j] = elem;
        tableauDirection[j] = element;
    }


	//On calcule la distance pour chaque direction et met dans un tableau trie

}

void Grid::removePerson(Person &p){
	for(int x=p.getX();x<p.getX()+DELTA;x++){
		for(int y=p.getY();y<p.getY()+DELTA;y++){
			this->grid[y][x].setStatus(FREE);
		}
	}
	p.setId(IGNORED);
}

bool Grid::canMove(int x, int y, int deltaX, int deltaY){
	int newX=x+deltaX;
	int newY=y+deltaY;


	//On vérifie si les cases vers lesquelles on veut se déplacer sont FREE
	if(deltaY==-1){
		for(int dx=0;dx<DELTA;dx++){
			if(this->grid[newY][newX+dx].getStatus()!=FREE){
				return false;
			}
		}
	}
	else if(deltaY==1){
		for(int dx=0;dx<DELTA;dx++){
			if(this->grid[y+DELTA][newX+dx].getStatus()!=FREE){
				return false;
			}
		}
	}
	if(deltaX!=0){
		for(int dy=0;dy<DELTA;dy++){
			if(this->grid[newY+dy][newX].getStatus()!=FREE){
				return false;
			}
		}
	}
	return true;
}

bool Grid::movePerson(Direction dir, Person &movingPerson){
	int deltaX=0;
	int deltaY=0;

	//cout << "Before movement: x: "<< movingPerson.m_x << " y: " << movingPerson.m_y <<endl;

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

	if(canMove(movingPerson.getX(),movingPerson.getY(),deltaX,deltaY)==false){
		return false;
	}



	int newX=movingPerson.getX()+deltaX;
	int newY=movingPerson.getY()+deltaY;

	//Déplacement vertical
	if(deltaY==-1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].setPerson(movingPerson);
			grid[newY][newX+x].setStatus(OCCUPIED);

			grid[newY+DELTA][movingPerson.getX()+x].setStatus(FREE);
		}
	}
	else if(deltaY==1){
		for(int x=0;x<DELTA;x++){
			grid[newY][newX+x].setPerson(movingPerson);
			grid[newY][newX+x].setStatus(OCCUPIED);

			grid[movingPerson.getY()][movingPerson.getX()+x].setStatus(FREE);
		}
	}
	//Déplacement horizental
	if(deltaX!=0){
		for(int y=0;y<DELTA;y++){
			grid[newY+y][newX].setPerson(movingPerson);
			grid[newY+y][newX].setStatus(OCCUPIED);

			grid[movingPerson.getY()+y][newX+DELTA].setStatus(FREE);

		}

	}
	//On deplace la personne
	movingPerson.movePerson(dir);
	//movingPerson.m_x+=deltaX;
	//movingPerson.m_y+=deltaY;
	//cout << "After movement: x: "<< movingPerson.m_x << " y: " << movingPerson.m_y <<endl;
	if(movingPerson.getX()==0) {
		removePerson(movingPerson);
	}
	return true;


	//Les autres directions ne sont pas utilisees dans ce programme
}


vector<Person> &Grid::getAllP(){
    return this->allp;
}

vector< vector<GridElement> > &Grid::getElements(){
    return this->grid;
}

int Grid::getAmountOfPeople(){
    return this->amountOfPeople;
}
