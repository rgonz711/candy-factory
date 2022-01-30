#include "io.h"
#include "belt.h"
#include "producer.h"
#include "production.h"
#include "consumer.h"
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <queue>

// using namespace std;

/*
Initialization of the "belt"/semaphores within belt struct
*/
BELT::BELT()
{
    candyProduced = candyConsumed = 0; 
    numFrogs = numSuckers = 0;
    sem_init(&unconsumed, 0, 0);
    sem_init(&maxFrogs, 0, CFB_MAX);
    sem_init(&spaceOnBelt, 0, BELT_MAX);
    sem_init(&maxProducers, 0, 0);
    sem_init(&nextCandy, 0, 1);


}






