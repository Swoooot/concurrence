#include "GridElement.h"
#include "Person.h"
#include <iostream>

using namespace std;
GridElement::GridElement()
{
    m_st=FREE;
}

GridElement::~GridElement()
{
    //dtor
}

void GridElement::setStatus(status st){
    this->m_st=st;
}

void GridElement::setPerson(Person p){
    this->m_p=p;
}

status GridElement::getStatus(){
    return this->m_st;
}

Person GridElement::getPerson(){
    return this->m_p;
}
