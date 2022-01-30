#include "belt.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer.h"
#include "production.h"
#include "io.h"

/*
Adding candy onto the belt
*/
int CFB_CANDY, EES_CANDY = 0;

/*
Function that creates a new producer and initializes the variables within the struct
*/
void BELT::PRODUCER_GEN(string candy_name, int delaytime)
{
    initialization init_producer;
    init_producer.str = candy_name;
    init_producer.time = delaytime;
    init_producer.finished = 0;
    init_producer.finishedSuckers = 0;
    init_producer.finishedFrogs = 0;
    
    //semaphore pointers
    init_producer.nextCandy = &nextCandy;
    init_producer.spaceOnBelt = &spaceOnBelt;
    init_producer.unconsumed = &unconsumed;
    init_producer.maxFrogs = &maxFrogs;
    init_producer.maxProducers = &maxProducers;

    init_producer.mizzo = this;

    producers.push_back(init_producer);
    numProducers++;
}

bool BELT::ON_BELT(string str)
{
    ProductType type1;

    //Checking to see if the maximum candy produced has been reached
    switch (candyProduced)
    {
    case 100:
        return false;
        break;
    }
    running_belt.push(str); 
    candyProduced++;
    //printf("Produced: %d\n", candyProduced);

    //If the current candy produced has not reached the max then
    //add the candy to the belt
    int compare = str.compare("crunchy frog bite");
    switch (compare)
    {
    case 0: //If the candy produced is a crunchy frog bite
        numFrogs++;
        CFB_CANDY++;
        type1 = FrogBite;
        break;
    default: //If the candy produced is a EES
        numSuckers++;
        EES_CANDY++;
        type1 = Escargot;
        break;
    }
    int num_candies[] = {numFrogs, numSuckers};
    int prod_candies[] = {CFB_CANDY, EES_CANDY};
    io_add_type(type1, num_candies, prod_candies);
    return true;
}

void* PRODUCE_CANDY(void* num) {
	//pthread_mutex_lock(&lock);
	initialization *producer = (initialization*)num;

	//using nanosleep for the input delay time 
	struct timespec delay_time;
	delay_time.tv_sec = producer->time / 1000;
	delay_time.tv_nsec = (producer->time % 1000) * 1000000;
	bool isCfb = (producer->str.compare("crunchy frog bite")==0);

	while (true) {
		nanosleep(&delay_time, NULL);

		//If the input is a CFB then a wait is put on producing if
		//the maximum amount of frogs is already on the belt
		switch(isCfb) {
			case 1:
				sem_wait(producer->maxFrogs);
				break;
		}
		sem_wait(producer->spaceOnBelt); //Checks to see if there is space for more candy on the belt

		sem_wait(producer->nextCandy); 
		bool check = (producer->mizzo->ON_BELT(producer->str)); //adds next candy onto the belt
		sem_post(producer->nextCandy);

		switch(check) {
			case 1:
				//if there is space on belt the candy it added
				sem_post(producer->unconsumed);
				producer->finished++;
				switch(isCfb) {
					case 1:
						producer->finishedFrogs++;
						break;
					default:
						producer->finishedSuckers++;
						break;
				}
				break;
			default:
				//If there is no space on the belt then the candy is not added
				pthread_exit(NULL);
				break;
		}
		if (!check){
			pthread_exit(NULL);
		}
	}
}