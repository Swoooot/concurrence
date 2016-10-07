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

using namespace std;

Grid grid;
vector< vector<GridElement> > &elements=grid.getElements();
vector<Direction> tableauDistance;


enum Mode{ONE_THREAD,FOUR_THREADS,VARIABLE};


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

void *variableThreads(bool executionTime){
    vector<pthread_t> tVector(grid.getAmountOfPeople());

    clock_t t;
    t=clock();

	struct timeval before, after;
	gettimeofday(&before , NULL);

    for(int i=0;i<grid.getAmountOfPeople();i++){
        pthread_create(&tVector[i],NULL,progressVariable,(void *)i);
    }

    for(int i=0;i<grid.getAmountOfPeople();i++){
        pthread_join(tVector[i],NULL);
    }

    t=clock()-t;
    double time_taken =((double)t)/CLOCKS_PER_SEC;

	gettimeofday(&after , NULL);
	double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;
    if(executionTime) cout << "Time required (" << grid.getAmountOfPeople() <<" threads): " << time_taken << endl;
	if(executionTime) cout << "Response Time required (" << grid.getAmountOfPeople() << " threads): " << respTime << endl;

}


void *progressFour(void *arg){
	vector<Direction> tableauDirection(5);
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
		for(int i=0;i<grid.getAmountOfPeople();i++){
			if(grid.getAllP()[i].getId()!=IGNORED){
				Person p=grid.getAllP()[i];
				if((p.getX()>=beginX && p.getX()<endX) && (p.getY()>=beginY && p.getY()<endY)){
					grid.shortestDistant(grid.getAllP()[i],0,AREA_WIDTH/2,tableauDirection);
					for(int j=0;j<tableauDirection.size();j++){
                       // cout << "Thread Id: " << threadId <<" Direction: " << tableauDirection[0] << " id: " << allp[i].m_id << " x: " << allp[i].m_x << " y: " << allp[i].m_y <<endl;
					//	printf("Dir: %d\n",tableauDirection[j]);
						bool move=grid.movePerson(tableauDirection[j],grid.getAllP()[i]);
						if(move==true) break;
					}
			//		cout << "Thread Id: " << threadId << " Direction: " << tableauDirection[0] << " Pid: " << grid.getAllP()[i].getId() << " x: " << grid.getAllP()[i].getX() << " y: " << grid.getAllP()[i].getY() <<endl;
				}
			}
			else {
				count++;
			}
		}
		if(count==grid.getAmountOfPeople()){
		//	cout << "All done!" << endl;
			return NULL;
		}
		else count=0;
	}


}



void fourThreads(bool executionTime){
	pthread_t pth0, pth1, pth2, pth3;

	clock_t t;
    t = clock();

	struct timeval before, after;
	gettimeofday(&before , NULL);
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

	gettimeofday(&after , NULL);
	double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;


    if(executionTime) cout << "Time required (4 threads): " << time_taken << endl;
	if(executionTime) cout << "Response Time required (4 threads): " << respTime << endl;
	
	return;



}



void progress(bool executionTime){
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
				//cout << "Direction: " << tableauDirection[0] << " id: " << grid.getAllP()[i].getId() << " x: " << grid.getAllP()[i].getX() << " y: " << grid.getAllP()[i].getY() <<endl;
			//	printf("direction: %d, id: %d, x: %d, y: %d\n",tableauDirection[0],allp[i].id,allp[i].x,allp[i].y);
			}
			else count++;
		}
		if(count==grid.getAmountOfPeople()){
		//	cout << "All done!" << endl;
			break;
		}
		else count=0;

		//cout << endl;
		//printf("\n");



	}

	//Calculer temps passé pour effectuer la simulation
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

	gettimeofday(&after , NULL);
	double respTime=(double) ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000000;


    if(executionTime) cout << "Time required (1 thread): " << time_taken << endl;
	if(executionTime) cout << "Response Time required (1 thread): " << respTime << endl;


}




int main(int argc, char* argv[])
{
    srand(time(NULL));
    int amountOfPeople=32; //Default value
    bool executionTime=false;
    Mode m=ONE_THREAD;

    //Parsing arguments
    if (argc >= 1 ){

		for (int i=0; argv[i] != NULL; i++){
			if ( argv[i][1]=='p'){

				int puissance = argv[i][2] - 48;

				amountOfPeople = pow(2,puissance);
			//	cout << amountOfPeople << endl;
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
	//cout << m << endl;



    /////////////////////////////////

    grid.fillGrid(amountOfPeople);

    switch(m){
        case ONE_THREAD:
            progress(executionTime);
            break;
        case FOUR_THREADS:
            fourThreads(executionTime);
            break;
        case VARIABLE:
            variableThreads(executionTime);
            break;
    }
	//





    return 0;
}
