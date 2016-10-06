#include "Person.h"
using namespace std;
Person::Person(int id, int x, int y)
{
    m_x=x;
    m_y=y;
    m_id=id;
    //ctor
}
Person::Person()
{
    m_x=0;
    m_y=0;
    m_id=-1;
    //ctor
}

Person::~Person()
{
    //dtor
}

void Person::movePerson(Direction d){
    switch(d){
        case NORTH:
            this->m_y-=1;
            break;
        case NORTHWEST:
            this->m_x-=1;
            this->m_y-=1;
            break;
        case WEST:
            this->m_x-=1;
            break;
        case SOUTHWEST:
            this->m_x-=1;
            this->m_y+=1;
            break;
        case SOUTH:
            this->m_y+=1;
            break;
    }

}

int &Person::getX(){
    return this->m_x;
}

int &Person::getY(){
    return this->m_y;
}

int &Person::getId(){
    return this->m_id;
}

void Person::setX(int x){
    this->m_x=x;
}

void Person::setY(int y){
    this->m_y=y;
}

void Person::setId(int id){
    this->m_id=id;
}
