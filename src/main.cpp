//AKHMADOV Baisangour
//LEVY Cyprien

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Person.h"
#include "GridElement.h"
#include "Grid.h"
#include "ThreadArg.h"
#include <sys/time.h>
#include <semaphore.h>

using namespace std;

Grid grid;
vector< vector<GridElement> > &elements=grid.getElements();
vector<Direction> tableauDistance;


enum Mode{ONE_THREAD,FOUR_THREADS,VARIABLE};
enum Step{ONE, TWO, THREE};

Step s;
bool executionTime;
sem_t sem;


void *progressVariable(void *arg){
    vector<Direction> tableauDirection(5);
    //On recupère le paramètre qui correspond à la personne qui est gérée par le thread
    Person p=*reinterpret_cast<Person*>(arg);

    while(true){
        if(p.getId()!=IGNORED){
            //On calcule les distances et effectue le deplacement
            grid.shortestDistant(p,0,AREA_WIDTH/2,tableauDirection);
            for(int j=0;j<tableauDirection.size();j++){
                if(s==TWO) sem_wait(&sem);
                bool move=grid.movePerson(tableauDirection[j],p);
                //cout << "Thread Id: " << p.getId() << " Direction: " << tableauDirection[0] << " id: " << p.getId() << " x: " << p.getX() << " y: " << p.getY() <<endl;
                if(s==TWO) sem_post(&sem);
                if(move==true) break;

            }
        }
        else{
            //La personne gérée par le thread a atteint la sortie
            return NULL;
        }
    }
}

//Creation un thread par personne et lancement simulation avec ceux-ci
void variableThreads(double &resp, double &cpu){
    vector<pthread_t> tVector(grid.getAmountOfPeople());
    vector<Person> &pVect=grid.getAllP();
    if(s==TWO) sem_init(&sem,0,1); //Si etape 2, initialisation de la semaphore

    //Variable temps
    clock_t t;
    t=clock();

    struct timeval before, after;
    gettimeofday(&before , NULL);




    for(int i=0;i<grid.getAmountOfPeople();i++){
        pthread_create(&tVector[i],NULL,progressVariable,&pVect[i]);
    }

    for(int i=0;i<grid.getAmountOfPeople();i++){
        pthread_join(tVector[i],NULL);
    }


    //Calcul temps
    t=clock()-t;
    double time_taken =((double)t)/CLOCKS_PER_SEC;

    gettimeofday(&after , NULL);
    double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;

    //Affichage temps
    if(executionTime){
        cout << "CPU Time required (" << grid.getAmountOfPeople() << " threads): " << time_taken << endl;
        cout << "Response Time required (" << grid.getAmountOfPeople() << " threads): " << respTime << endl;
        resp=respTime;
        cpu=time_taken;
    }

}


void *progressFour(void *arg){

    vector<Direction> tableauDirection(5);

    ThreadArg &ta= *reinterpret_cast<ThreadArg*>(arg); //Cet objet contient les limites des coordonnées gérées par le thread


    while(true){
        vector<Person *> pVect;

        //On détermine les personnes que le thread déplace
        if(s==TWO) sem_wait(&sem); //Si on est dans l'étape 2 on fait down sur la semaphore ici
        int leftCount=grid.getAmountOfPeople();
        for(int i=0;i<grid.getAmountOfPeople();i++){

            if(grid.getAllP()[i].getX() >= ta._beginX && grid.getAllP()[i].getX() <= ta._endX && grid.getAllP()[i].getY() >= ta._beginY && grid.getAllP()[i].getY() <= ta._endY){
                pVect.push_back(&grid.getAllP()[i]);
            }

            if(grid.getAllP()[i].getId()==IGNORED) {
                leftCount--;
            }

        }
        if(s==TWO) sem_post(&sem); //On libere la semaphore


        if(leftCount==0) { //Si toutes les personnes sont arrivees a destination on arrete la fonction
                return NULL;
        }

        //On déplace les personnes
        for(int i=0;i<pVect.size();i++){
            if(pVect[i]->getId()!=IGNORED){
                Person *p=pVect[i];


                    grid.shortestDistant(*p,0,AREA_WIDTH/2,tableauDirection);
                    for(int j=0;j<tableauDirection.size();j++){

                        bool move=grid.movePerson(tableauDirection[j],*p);
                        if(move==true) break;
                    }


            }
        }

    }


}


//Fonction qui cree les 4 threads et lance la simulation 4 threads
void fourThreads(double &resp, double &cpu){
    if(s==TWO) sem_init(&sem,0,1); //Si etape 2, initialisation de la semaphore
    int tAmount=4;
    vector<pthread_t> tVector(tAmount);

    //Variables temps
    clock_t t;
    t=clock();

    struct timeval before, after;
    gettimeofday(&before , NULL);

    //Creation parametres de thread (contient les limites de coordonnes pour le thread)
    vector<ThreadArg> threadArgs;


    ThreadArg t1(0,0,AREA_LENGTH/2-1,AREA_WIDTH/2-1);
    ThreadArg t2(AREA_LENGTH/2,0,AREA_LENGTH-1,AREA_WIDTH/2-1);
    ThreadArg t3(0,AREA_WIDTH/2,AREA_LENGTH/2-1,AREA_WIDTH-1);
    ThreadArg t4(AREA_LENGTH/2,AREA_WIDTH/2,AREA_LENGTH-1,AREA_WIDTH-1);

    threadArgs.push_back(t1);
    threadArgs.push_back(t2);
    threadArgs.push_back(t3);
    threadArgs.push_back(t4);


    for(int i=0;i<tAmount;i++){
        pthread_create(&tVector[i],NULL,progressFour,&threadArgs[i]);
    }

    for(int i=0;i<tAmount;i++){
        pthread_join(tVector[i],NULL);
    }


    //Calcul temps
    t=clock()-t;
    double time_taken =((double)t)/CLOCKS_PER_SEC;

    gettimeofday(&after , NULL);
    double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;

    //Affichage temps
    if(executionTime){
        cout << "CPU Time required (" << tAmount << " threads): " << time_taken << endl;
        cout << "Response Time required (" << tAmount << " threads): " << respTime << endl;
        resp=respTime;
        cpu=time_taken;
    }

    return;

}


//Simulation 1 thread
void progress(double &resp, double &cpu){
    vector<Direction> tableauDirection(5);
    int count=0;
    //Compteur temps
    clock_t t;
    t = clock();

    struct timeval before, after;
    gettimeofday(&before , NULL);

    while(true){

        for(int i=0;i<grid.getAmountOfPeople();i++){
            if(grid.getAllP()[i].getId()!=IGNORED){
                grid.shortestDistant(grid.getAllP()[i],0,AREA_WIDTH/2,tableauDirection);
                for(int j=0;j<tableauDirection.size();j++){
                    bool move=grid.movePerson(tableauDirection[j],grid.getAllP()[i]);
                    if(move==true) break;
                }

            }
            else count++;
        }
        if(count==grid.getAmountOfPeople()){
        //  cout << "All done!" << endl;
            break;
        }
        else count=0;


    }

    //Calculer temps passé pour effectuer la simulation
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    gettimeofday(&after , NULL);
    double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;



    if(executionTime){
        cout << "CPU Time required (1 thread): " << time_taken << endl;
        cout << "Response Time required (1 thread): " << respTime << endl;
        resp=respTime;
        cpu=time_taken;
    }


}




int main(int argc, char* argv[])
{
    int amountOfPeople=32; //Default value
    executionTime=false;
    Mode m=ONE_THREAD;
    s=ONE;


    //Parsing arguments
    if (argc >= 1 ){

        for (int i=0; argv[i] != NULL; i++){
            if ( argv[i][1]=='p'){

                int puissance = argv[i][2] - 48;

                amountOfPeople = pow(2,puissance);
            }
            if (argv[i][1]=='t'){
                if (argv[i][2] == '0'){
                    m=ONE_THREAD;
                }
                if (argv[i][2] == '1'){
                    m=FOUR_THREADS;
                }
                if (argv[i][2] == '2'){
                    m=VARIABLE;
                }

            }
            if (argv[i][1]=='m'){
                executionTime = true;
            }
            if (argv[i][1]=='e'){
                if (argv[i][2] == '1'){
                    s=ONE;
                }
                if (argv[i][2] == '2'){
                    s=TWO;
                }
                if (argv[i][2] == '3'){
                    s=THREE;
                }

            }
        }
    }

    //Affichage paramètres
    cout << "Etape : " << s+1 << endl;
    switch(m){
        case ONE_THREAD:
            cout << "Mode : 1 thread" << endl;
            break;
        case FOUR_THREADS:
            cout << "Mode : 4 threads" << endl;
            break;
        case VARIABLE:
            cout << "Mode : 1 thread par personne" << endl;
            break;
    }
    cout << "Nombre de personnes : " << amountOfPeople << endl;
    if(executionTime) cout << "Mesure du temps activee" << endl;
    else cout << "Aucune mesure" << endl;
    cout << endl;


    /////////////////////////////////

    double resp;
    double cpu;
    //Execution avec mesures temps
    if(executionTime){
        double meanResp=0;
        double meanCpu=0;
        for(int i=0;i< 5;i++){
            cout << "Mesure " << i+1 << endl;
            if(i>0){
                Grid reset;
                grid=reset;
            }
            grid.fillGridFixed(amountOfPeople);

            switch(m){
                case ONE_THREAD:
                    progress(resp,cpu);
                    break;
                case FOUR_THREADS:
                    fourThreads(resp,cpu);
                    break;
                case VARIABLE:
                    variableThreads(resp,cpu);
                    break;
            }
            if(i>=1 && i <=3){
                meanResp+=resp;
                meanCpu+=cpu;
            }
        }
        meanResp/=3;
        meanCpu/=3;
        cout << endl;
        cout << "Mean Response Time: " << meanResp << endl;
        cout << "Mean CPU Time: " << meanCpu << endl;
    }
    //Execution sans mesures
    else{

        grid.fillGridFixed(amountOfPeople);
        switch(m){
            case ONE_THREAD:
                progress(resp,cpu);
                break;
            case FOUR_THREADS:
                fourThreads(resp,cpu);
                break;
            case VARIABLE:
                variableThreads(resp,cpu);
                break;
        }
    }



    return 0;
}
