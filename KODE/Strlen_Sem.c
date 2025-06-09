#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
// #include "common.h"
// #include "common_threads.h"
#define MAXWORDLEN 16 //15+1
#define WORKERS 3
#define INPUTLENGTH 57
const char* input[INPUTLENGTH]={"Man","skal", "holde","tungen","lige","i","munden","n˚ar",
"man", "skriver","programmer", "med","parallelitet", "eller","med", "en", "bedre",
"term","concurrency" ,"Der","kan","være","mange","fælder","s˚asom","race","conditions",
"deadlocks","og", "udsultning","Det","er","derfor","vigtigt","at","man","sætter","sig",
"grundigt", "ind","i","hvordan","synkronisering","og","gensidig","udelukkelse",
"fungerer","og", "bedst", "anvendes", "s˚a","ens","programmer","bliver","effektive","og",
"korrekte"};
int counts[MAXWORDLEN];
//Mutex og synkroniserings erklaeringer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;


void initSemaphores() {
sem_init(&sem, 0, 0);
pthread_mutex_init(&mutex, NULL);
}


void getChunk(long workerID, int * start, int * end){
    int chunkSize = INPUTLENGTH / WORKERS;
    *start=workerID*chunkSize;
    *end=(workerID+1)*chunkSize;
    //worker with highest id gets remaining elems
    if(workerID==WORKERS-1) *end=INPUTLENGTH;
}


void *myWorker(void *arg) {
    long id=(long)arg;
    int start=0;int end=0;
    getChunk(id,&start,&end);
    printf("traad id %ld: start %d, end %d\n",id,start,end);
    //COUNT
    for(int i=start;i<end;i++) {
        int wordLen=strlen(input[i]);
        pthread_mutex_lock(&mutex);
        counts[wordLen]++;
        pthread_mutex_unlock(&mutex);
    }
    printf("traad id %ld: done counting\n",id);
    sem_post(&sem); // Signal that this thread is ready to compute stats


    return NULL;
}




void *computeStats(void *arg) {
    printf("computeStats thread started\n");
    long id=(long)arg;
    printf("traad id %ld: \n",id);
    //WAIT for Workers done
    for(int i=0;i<WORKERS;i++) {
        sem_wait(&sem); // Wait for each worker to signal completion
    }


    for(int i=0;i<MAXWORDLEN;i++) {
        printf("%d:\t",i);
    for(int j=0;j<counts[i];j++) printf("#");
        printf("\n");
    }
    return NULL;
}




int main(int argc, char *argv[]) {
    initSemaphores();
    pthread_t threads[WORKERS + 1];
   
    for (int i = 0; i < WORKERS; i++) {
        printf("Creating worker thread %d\n", i);
        pthread_create(&threads[i], NULL, myWorker, (void*)i);
    }


    pthread_create(&threads[WORKERS], NULL, computeStats, (void*)4);


    // Wait for all threads to finish
    for (int i = 0; i < WORKERS + 1; i++) {
        pthread_join(threads[i], NULL);
    }


    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);


    return 0;
}


