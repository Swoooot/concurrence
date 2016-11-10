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
#include <sys/time.h>
#include <semaphore.h>

using namespace std;

Grid grid;
vector< vector<GridElement> > &elements=grid.getElements();
vector<Direction> tableauDistance;


enum Mode{ONE_THREAD,FOUR_THREADS,VARIABLE};

bool executionTime;
sem_t sem;


void *progressVariable(void *arg){
    vector<Direction> tableauDirection(5);
    int personId=(int) arg;

    while(true){
        if(grid.getAllP()[personId].getId()!=IGNORED){
            grid.shortestDistant(grid.getAllP()[personId],0,AREA_WIDTH/2,tableauDirection);
            for(int j=0;j<tableauDirection.size();j++){
                bool move=grid.movePerson(tableauDirection[j],grid.getAllP()[personId]);
                if(move==true) break;

            }
        }
        else{
            return NULL;
        }
    }
}

void variableThreads(double &resp, double &cpu){
    vector<pthread_t> tVector(grid.getAmountOfPeople());

    clock_t t;
    t=clock();

    struct timeval before, after;
    gettimeofday(&before , NULL);

    for(int i=0;i<grid.getAmountOfPeople();i++){
        intptr_t id=i;
        pthread_create(&tVector[i],NULL,progressVariable,(void *)(id));
    }

    for(int i=0;i<grid.getAmountOfPeople();i++){
        pthread_join(tVector[i],NULL);
    }

    t=clock()-t;
    double time_taken =((double)t)/CLOCKS_PER_SEC;

    gettimeofday(&after , NULL);
    double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;

    if(executionTime){
        cout << "Time required (" << grid.getAmountOfPeople() << " threads): " << time_taken << endl;
        cout << "Response Time required (" << grid.getAmountOfPeople() << " threads): " << respTime << endl;
        resp=respTime;
        cpu=time_taken;
    }

}


void *progressFour(void *arg){

	vector<Direction> tableauDirection(5);
	int count=0;

	int threadId=(int) arg;

	int beginX, beginY = 0;
	int endX, endY = 0;

	//Intervalles de coordonn�es g�r�es selon le thread
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
		for(int i=0;i<grid.getAmountOfPeople();i++){
			if(grid.getAllP()[i].getId()!=IGNORED){
				Person p=grid.getAllP()[i];
				if((p.getX()>=beginX && p.getX()<endX) && (p.getY()>=beginY && p.getY()<endY)){

                  //  sem_wait(&sem); //Semaphore attente
					grid.shortestDistant(grid.getAllP()[i],0,AREA_WIDTH/2,tableauDirection);
					for(int j=0;j<tableauDirection.size();j++){
                        if(!executionTime){
                            sem_wait(&sem); //Semaphore attente
                            cout << "Thread Id: " << threadId << " Direction: " << tableauDirection[0] << " id: " << grid.getAllP()[i].getId() << " x: " << grid.getAllP()[i].getX() << " y: " << grid.getAllP()[i].getY() <<endl;
                            sem_post(&sem);
                        }
					//	printf("Dir: %d\n",tableauDirection[j]);
						bool move=grid.movePerson(tableauDirection[j],grid.getAllP()[i]);
						if(move==true) break;
					}
                    //sem_post(&sem); //Semaphore liberation

			//		cout << "Thread Id: " << threadId << " Direction: " << tableauDirection[0] << " Pid: " << grid.getAllP()[i].getId() << " x: " << grid.getAllP()[i].getX() << " y: " << grid.getAllP()[i].getY() <<endl;
				}
			}
			else {
				count++;
			}
		}
       // updateDisplay();
		if(count==grid.getAmountOfPeople()){
			return NULL;
		}
		else count=0;
	}


}



void fourThreads(double &resp, double &cpu){
    sem_init(&sem,0,1);
    int tAmount=4;
    vector<pthread_t> tVector(tAmount);

    clock_t t;
    t=clock();

    struct timeval before, after;
    gettimeofday(&before , NULL);

    for(int i=0;i<tAmount;i++){
        intptr_t id=i;
        pthread_create(&tVector[i],NULL,progressFour,(void *)(id));
    }

    for(int i=0;i<tAmount;i++){
        pthread_join(tVector[i],NULL);
    }

    t=clock()-t;
    double time_taken =((double)t)/CLOCKS_PER_SEC;

    gettimeofday(&after , NULL);
    double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;

    if(executionTime){
        cout << "Time required (" << tAmount << " threads): " << time_taken << endl;
        cout << "Response Time required (" << tAmount << " threads): " << respTime << endl;
        resp=respTime;
        cpu=time_taken;
    }

    return;

}



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
			//	cout << "Direction: " << tableauDirection[0] << " id: " << grid.getAllP()[i].getId() << " x: " << grid.getAllP()[i].getX() << " y: " << grid.getAllP()[i].getY() <<endl;
			//	printf("direction: %d, id: %d, x: %d, y: %d\n",tableauDirection[0],allp[i].id,allp[i].x,allp[i].y);
			}
			else count++;
		}
		if(count==grid.getAmountOfPeople()){
		//	cout << "All done!" << endl;
			break;
		}
		else count=0;


	}

	//Calculer temps pass� pour effectuer la simulation
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
		}
	}

	//Affichage param�tres
	switch(m){
        case ONE_THREAD:
            cout << "Mode: 1 thread" << endl;
            break;
        case FOUR_THREADS:
            cout << "Mode: 4 threads" << endl;
            break;
        case VARIABLE:
            cout << "Mode: 1 thread par personne" << endl;
            break;
	}
    cout << "Nombre de personnes: " << amountOfPeople << endl;
    if(executionTime) cout << "Mesure du temps activee" << endl;
    else cout << "Aucune mesure" << endl;
    cout << endl;


    /////////////////////////////////

    double resp;
    double cpu;
    if(executionTime){
        double meanResp=0;
        double meanCpu=0;
        for(int i=0;i< 5;i++){
            cout << "Mesure " << i+1 << endl;
            Grid reset;
            grid=reset;
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
