/********************************************************************
CSCI 480 - Assignment 4 - Spring 2022

Programmer: Justin McLain
ZID  Z1910087	   
Section:   PE1
Date Due:  4/4/2022

Purpose: solve the Producer-Consumer problem using the PThreads 
library, will get to practice mutex and semaphores in PThreads
Library.
*********************************************************************/

#include<iostream>
#include<semaphore.h>
#include<stdio.h>
#include<iomanip>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>



#define Producer_Steps 5//producer thread iterations
#define Consumer_Steps 5 //consumer thread iterations
#define NUM_THREADS 4 //count of threads
#define BUFFER_SIZE 10 //max capacity of buffer


void * Produce(void *threadIn);
void * Consume(void *ThreadIn);
void Insert(int threadIn);
void Remove(int threadIn);

sem_t full;
sem_t empty;
int cookieCount;

pthread_mutex_t mutex;

// Main 
// executes thread code from codeIn 
//error checks for proper input
int main(int argc, char *argv[]){
    int ret_creats;

    //check semaphore containments for max and empty
    sem_init(&full, 0, BUFFER_SIZE);
    sem_init(&empty, 0, 0);

    //make the consumer and produce array for threads
    pthread_t Consumer[NUM_THREADS];
    pthread_t Producer[NUM_THREADS];

    //define the mutex
    pthread_mutex_init(&mutex,NULL);

    long c = 0;
	while (c < NUM_THREADS){	
		ret_creats = pthread_create(&Consumer[c], NULL, Consume, (void *) c);	
		if(ret_creats){	
            //error check for test case
			std::cerr << "Error in Consumer create" << std::endl;
			exit(-1);
		}
        c++;
	}

        long p = 0;
	while (p < NUM_THREADS){	
		ret_creats = pthread_create(&Producer[p], NULL, Produce, (void *) p);
		if(ret_creats){	
            //error check for test case
			std::cerr << "Error in producer create" << std::endl;
			exit(-1);
		}
        p++;
	}

    //this deletes the total number of threads
    //once the endpoint of Consumer is reached
    int k = 0;    
	while (k < NUM_THREADS){	
		pthread_join(Consumer[k], NULL);
        k++;
	}
    
    //this deletes the total number of threads
    //one the endpoint of Producer is reaches
    int l = 0;
	while(l < NUM_THREADS){	
		pthread_join(Producer[l], NULL);
        l++;
	}

    //end print statement
    std::cout << "All threads are done." << std::endl;
	std::cout << "Resources cleaned up." << std::endl;

    pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);

	// delete the semaphores.
	sem_destroy(&full);	
	sem_destroy(&empty);	

	return 0;
}


//this function inserts, adds, and configures
// threads to add one 
void Insert(int threadIn){
    int codeIn;
    //attempt mutex lock
    codeIn = pthread_mutex_lock(&mutex);

    //error check #1
    if(codeIn !=0){
        std::cerr << "producer #" << threadIn << "Error in the lock" << std::endl;
        exit(-1);
    }
    else{
        cookieCount++;
        std::cerr <<"producer #" << threadIn << " inserted a cookie." << " Total: " << cookieCount << std::endl;
    }

    //this step unlocks the mutex
    codeIn = pthread_mutex_unlock(&mutex);

    //if error then halt the program and quit
    //otherwise just disregard
    if(codeIn != 0){
        std::cerr << "producer #" << threadIn << " Error in the Unlock" << std::endl;
    }

}

//this function will take the cookieCount out
// of the counter and lock and relock & repeat
void Remove(int threadIn) {
    //placeholder for the pthread_mutex
    //and lock
    int codeIn = pthread_mutex_lock(&mutex);
    //error check for lock
    if (codeIn != 0) {
        std::cerr <<"consumer #" << threadIn << ": Failed in Remove: lock" << std::endl;
        exit(-1);
    }
    else {
        //decrement the counter
        cookieCount--;
        std::cerr << "consumer #" << threadIn << " removed a cookie. " << "Total: " << cookieCount << std::endl; 
    }

    //this unlocks the mutex when the function is being called
    codeIn = pthread_mutex_unlock(&mutex);

    //error check for unlock
    if (codeIn != 0) {
        std::cerr << "consumer: #" << threadIn << ": Failed in Remove Unlock" << std::endl;
        exit(-1);
    }
}

//producer thread will alternate between inserting 
//an item and sleeping for 1 second. It will do so for 5 
//times. Call sleep(1) to sleep for 1 second between iterations.

void * Produce(void *threadIn) {
    long temp = (long)threadIn;

    int i = 0;
    while (i < Producer_Steps) {
        sem_wait(&full);
        Insert(temp);
        sem_post(&empty);
        //sleep and increment
        sleep(1);
        i++;
    }
    //finish
    pthread_exit(NULL);
}

//cosumer thread runs eacdh thread to delete int
//countwidbuffer

void * Consume(void * threadIn){
    //convert value to int
    long temp = (long)threadIn;

    int i = 0;
    while (i < Consumer_Steps) {
        sem_wait(&empty);
        Remove(temp);
        sem_post(&full);
        //sleep and increment
        sleep(1);
        i++;
    }
    //finish
    pthread_exit(NULL);
}