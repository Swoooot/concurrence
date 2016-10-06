#include "Person.h"

#ifndef GRIDELEMENT_H
#define GRIDELEMENT_H

using namespace std;
enum status{FREE,OCCUPIED};
class GridElement
{
    public:
        GridElement();


        virtual ~GridElement();

        void setStatus(status st);
        void setPerson(Person p);
        status getStatus();
        Person getPerson();

    protected:

    private:
        Person m_p;
        status m_st;
};

#endif // GRIDELEMENT_H
