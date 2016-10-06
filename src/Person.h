#ifndef PERSON_H
#define PERSON_H

using namespace std;
enum Direction{NORTH,NORTHWEST,WEST,SOUTHWEST,SOUTH};
class Person
{
    public:
        Person();
        Person(int id);
        Person(int x,int y,int id);

        virtual ~Person();

        void movePerson(Direction d);
        int &getX();
        int &getY();
        int &getId();
        void setX(int x);
        void setY(int y);
        void setId(int id);

    protected:

    private:
        int m_x;
        int m_y;
        int m_id;
};

#endif // PERSON_H
