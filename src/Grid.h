//AKHMADOV Baisangour
//LEVY Cyprien

#include <vector>
#include "Person.h"
#include "GridElement.h"
using namespace std;

#ifndef GRID_H
#define GRID_H

const int AREA_LENGTH=512;
const int AREA_WIDTH=128;
const int DELTA=4;
const int IGNORED=-1;


class Grid
{
    public:

        Grid();
        virtual ~Grid();

        void initializeGrid();
        void addPerson(int x ,int y, int id);
        void fillGrid(int amountOfPeople);
        void shortestDistant(Person p, int goalX, int goalY, vector<Direction> &tableauDirection);
        bool canMove(int x, int y, int deltaX, int deltaY);
        void removePerson(Person &p);
        bool movePerson(Direction dir, Person &movingPerson);

        vector<Person> &getAllP();
        vector< vector<GridElement> > &getElements();
        int getAmountOfPeople();

    protected:

    private:
        vector< vector<GridElement> > grid;
        vector<Person> allp;
        int amountOfPeople;
};

#endif // GRID_H
