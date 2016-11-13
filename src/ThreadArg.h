//AKHMADOV Baisangour
//LEVY Cyprien

#ifndef THREADARG_H
#define THREADARG_H


class ThreadArg
{
    public:
        ThreadArg(int bx,int by,int ex,int ey);
        int _beginX;
        int _beginY;
        int _endX;
        int _endY;
        virtual ~ThreadArg();

    protected:

    private:
};

#endif // THREADARG_H
