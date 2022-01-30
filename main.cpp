/*
 CS 570
 Jaylan Pierce, RedID 822644713
 Raul Gonzalez, RedID 824258754
 Assignment 4 - Part 2
 Professor Ben Shen
 April 20, 2021
 */

#include "main.h"
#include "belt.h"
#include "producer.h"
#include "consumer.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <semaphore.h>
#include <getopt.h>

/*
pthread creator function to create the produce and consumer threads
*/
void BELT::creator()
 {
    //Crunchy Frog Bits Generator
    pthread_t produced_item;
    initialization *thr_CFB = &(producers)[0];
    pthread_create(&produced_item, NULL, PRODUCE_CANDY, (void *)thr_CFB);

    //Everlasting Escargo Sucker Generator
    initialization *thr_EES = &(producers)[1];
    pthread_create(&produced_item, NULL, PRODUCE_CANDY, (void *)thr_EES);

    //Lucy Consumer Thread
    pthread_t consumer_item;
    initialization *thr_L = &(consumers)[0];
    pthread_create(&consumer_item, NULL, CONSUME_CANDY, (void *)thr_L);

    //Ethel Consumer Thread
    initialization *thr_E = &(consumers)[1];
    pthread_create(&consumer_item, NULL, CONSUME_CANDY, (void *)thr_E);

}

/*
uses production.h to print out production report summary at end
*/
void BELT::production()
{
    //Puts all calculated data into a 2d array to be accepted by the parameter 'io_production_report'
    int TOTAL_PRODUCED[2];
    TOTAL_PRODUCED[0]= producers[0].finished;
    TOTAL_PRODUCED[1]= producers[1].finished;

    int ETHEL_CONSUMED[2];
    ETHEL_CONSUMED[0] = consumers[0].finishedSuckers;
    ETHEL_CONSUMED[1] = consumers[0].finishedFrogs;

    int LUCY_CONSUMED[2];
    LUCY_CONSUMED[0] = consumers[1].finishedSuckers;
    LUCY_CONSUMED[1] = consumers[1].finishedFrogs;

    int *TOTAL_CONSUMED[2];
    TOTAL_CONSUMED[0]= LUCY_CONSUMED;
    TOTAL_CONSUMED[1]= ETHEL_CONSUMED;

    //Printing the Production Report
    io_production_report(TOTAL_PRODUCED, TOTAL_CONSUMED);
}


int main(int argc, char *argv[])
{
    opt_args delayers;
    BELT mizzo;
    int option;

    //Reading in optional given command line arguements.
    //If a arguement is not given then it defaults to 0
    while ((option = getopt(argc, argv, "E:L:f:e:")) != -1)
    {
        switch (option)
        {
        case 'E':
            if (atoi(optarg) != NULL)
                delayers.E_C_MS = atoi(optarg);
            break;

        case 'L':
            if (atoi(optarg) != NULL)
                delayers.L_C_MS = atoi(optarg);
            break;

        case 'f':
            if (atoi(optarg) != NULL)
                delayers.CFB_P_MS = atoi(optarg);
            break;

        case 'e':
            if (atoi(optarg) != NULL)
                delayers.EES_P_MS = atoi(optarg);
            break;

        default:
            break;
        }
    }

    //Putting given time inputs into each producer and consumer
    mizzo.PRODUCER_GEN("crunchy frog bite", delayers.CFB_P_MS);
    mizzo.PRODUCER_GEN("everlasting escargot sucker", delayers.EES_P_MS);
    mizzo.CONSUMER_GEN("Ethel", delayers.E_C_MS);
    mizzo.CONSUMER_GEN("Lucy", delayers.L_C_MS);

    //Creating pthreads for consumers and producers
    mizzo.creator();
    //Semaphore to wait for all candies to be produced before letting main continue
    sem_wait(&mizzo.maxProducers);
    //Output
    mizzo.production();
}