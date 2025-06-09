#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
int ready_count = 0; 
// void initSemaphores() {}
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
        pthread_mutex_lock(&mutex); // Lock mutex to ensure thread safety
        counts[wordLen]++;
        pthread_mutex_unlock(&mutex); // Unlock mutex after updating shared data
    }
    printf("traad id %ld: done\n",id);

    // Signal that this thread is done counting
    pthread_mutex_lock(&mutex);
    ready_count++;

    if (ready_count == WORKERS) {
        pthread_cond_signal(&cond); // Signal the stats thread that all workers are done
    }
        pthread_mutex_unlock(&mutex);

    //DONE
    return NULL;
}
void *computeStats(void *arg) {
    pthread_mutex_lock(&mutex); // Lock mutex to safely access shared data
    while (ready_count < WORKERS) {
        pthread_cond_wait(&cond, &mutex); // Wait until all workers are done
    }
    pthread_mutex_unlock(&mutex); // Unlock mutex after waiting
    
    printf("Stats thread started\n");
    long id=(long)arg;
    printf("traad id %ld: \n",id);

    for(int i=0;i<MAXWORDLEN;i++) {
        printf("%d:\t",i);
        for(int j=0;j<counts[i];j++) printf("#");
        printf("\n");
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t threads[WORKERS + 1];

    // Create worker threads
    for (int i = 0; i < WORKERS; i++) {
        pthread_create(&threads[i], NULL, myWorker, (void*)(long)i);
    }

    // Create computeStats thread
    pthread_create(&threads[WORKERS], NULL, computeStats, (void*)4);

    // Join all threads
    for (int i = 0; i < WORKERS + 1; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond); 

    return 0;
}
