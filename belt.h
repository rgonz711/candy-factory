#ifndef BELT_H_INCLUDED
#define BELT_H_INCLUDED

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <queue>
#include <string>
#include <iostream>

using namespace std;
#define PRODUCTION_MAX 100
#define BELT_MAX 10
#define CFB_MAX 3


typedef struct BELT{
	//function prototypes

	void creator();
	void production();
	BELT();
	bool ON_BELT(string type);
	string OFF_BELT(string type);
	void PRODUCER_GEN(string type, int delay);
	void CONSUMER_GEN(string type, int delay);
	
	//Initializations
	int candyProduced, candyConsumed, numFrogs, numSuckers, numProducers, numConsumers;
	queue<string> running_belt;
	vector<struct initialization> producers, consumers;

	//Semaphores
	sem_t unconsumed; //unconsumed candy on belt
	sem_t maxFrogs; //maximum frogs on belt
	sem_t maxProducers; //maximum candy that can be produced
	sem_t nextCandy; //next candy to be put on belt
	sem_t spaceOnBelt; //space on belt



 };

//attributes
typedef struct initialization {
	//Initializations
	string str;
	int time, finished, finishedSuckers, finishedFrogs;
	BELT* mizzo;
	
	//Semaphores
	sem_t* unconsumed; //unconsumed candy on belt
	sem_t* maxFrogs; //maximum frogs on belt
	sem_t* maxProducers; //maximum candy that can be produced
	sem_t* nextCandy; //next candy to be put on belt
	sem_t* spaceOnBelt; //space on belt
};



#endif
