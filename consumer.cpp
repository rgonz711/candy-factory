#include "io.h"
#include "consumer.h"
#include "belt.h"
#include "production.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>



/*
Function that creates a new consumer and initializes the variables within the struct
*/
void BELT::CONSUMER_GEN(string candy_name, int delaytime)
{
    initialization init_consumer;
    init_consumer.str = candy_name;
    init_consumer.time = delaytime;
    init_consumer.finished = 0;
    init_consumer.finishedSuckers = 0;
    init_consumer.finishedFrogs = 0;
    
    //semaphore pointers
    init_consumer.nextCandy = &nextCandy;
    init_consumer.spaceOnBelt = &spaceOnBelt;
    init_consumer.unconsumed = &unconsumed;
    init_consumer.maxFrogs = &maxFrogs;
    init_consumer.maxProducers = &maxProducers;

    init_consumer.mizzo = this;

    consumers.push_back(init_consumer);
    numConsumers++;
}

/*
works with io_remove_type function to print consumer thread work
*/
string BELT::OFF_BELT(string str)
{
    string product = "";
    ConsumerType type1;
    ProductType type2;
    int candyOnBelt[] = {numFrogs, numSuckers};
    int candyConsumedLucy[] = {consumers[1].finishedSuckers + 1, consumers[1].finishedFrogs}; 
    int candyConsumedEthel[] = {consumers[0].finishedSuckers + 1, consumers[0].finishedFrogs};

    //Checking if the maximum candy consumed has been reached
    switch (candyConsumed)
    {
    case 100:
        return "MAX";
        break;
    }

    string get_candy = running_belt.front();
    running_belt.pop();
    candyConsumed++;

    bool CFB = get_candy.compare("crunchy frog bite") == 0;

    //If the current candy consumed has not reached the max then
    //remove the candy off the belt
    switch (CFB)
    {
    case 1: //IF the removed candy is a CFB decrement --
        numFrogs--;
        product = "crunchy frog bite";
        type2 = FrogBite;
        break;
    case 0: //IF the removed candy is a EES decrement --
        numSuckers--;
        product = "everlasting escargot sucker";
        type2 = Escargot;
    }
    
    //using production.h to read in stinrg name and compare const int for Lucy otherwise gives Ethel
    int compare = str.compare("Lucy");
    switch (compare)
    {
    case 0:
        type1 = Lucy;
        // calls io to remove and print
        io_remove_type(type1, type2, candyOnBelt, candyConsumedLucy);
        break;
    default:
        type1 = Ethel;  
        //calls io to remove and print                                             
        io_remove_type(type1, type2, candyOnBelt, candyConsumedEthel);
        break;
    }

    switch (candyConsumed)
    {
        //used to stop production past 100 candies
    case 100:
        sem_post(&maxProducers);
        break;
    }
    return get_candy;
}

void* CONSUME_CANDY(void* num){
	//pthread_mutex_lock(&lock2);
	initialization* consumer = (initialization*)num;
	struct timespec delay_time;
	delay_time.tv_sec = consumer->time / 1000;
	delay_time.tv_nsec = (consumer->time % 1000) * 1000000;

	while (true) {
		//Using nanosleep for input delay
		nanosleep(&delay_time, NULL);
		//Checks if there is unconsumed on belt
		sem_wait(consumer->unconsumed);
		sem_wait(consumer->nextCandy);
		string typeCandy = consumer->mizzo->OFF_BELT(consumer->str); //Takes next candy off of the belt
		sem_post(consumer->nextCandy);
	
		int compare = typeCandy.compare("MAX");
		int compare2 = typeCandy.compare("crunchy frog bite");

		switch(compare){
			case 0:
				//If maximum consumed has been reached then thread is exited
				pthread_exit(NULL);
				break;
			default:
				//Checks if there is space on belt
				sem_post(consumer -> spaceOnBelt);
				//If the candy is CFB it increments the number of frogs consumed
				if (compare2 == 0){
					sem_post(consumer->maxFrogs);
					consumer->finishedFrogs++;
				}
				//If the candy is EES it increments the number of suckers consumed
				else{
					consumer->finishedSuckers++;
				}
				consumer->finished++;
		}

	}
	//pthread_mutex_unlock(&lock2);

}