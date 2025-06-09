#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// #include "common.h"
// #include "common_threads.h"

const int N=3;
long arr[9]={1,2,3, 4,5,6, 7,8,9};
long sum[3]={0,0,0};

//Mutex og condition erklaeringer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
int ready_count = 0;


void *mythread(void *arg) {
    long id=(long)arg;
    printf("Traad %ld: start\n", id);
    long total=0; 
    for(int i=0;i<N;i++){
        total+= arr[id*N+i];
    }
    sum[id]=total;
    printf("Traad %ld: foer: %ld\n", id, total);
    pthread_mutex_lock(&mutex);
    ready_count++;
    printf("%d: ready_count %d\n", id, ready_count);
    if (ready_count == N) {
        pthread_cond_broadcast(&cond); // Signal the stats thread that all workers are done
        printf("All workers are ready, signaling condition variable\n");
    
    }

    while(ready_count < N) {
        printf("Traad %ld: venter, ready_count %d\n", id, ready_count);
        pthread_cond_wait(&cond, &mutex); // Wait until all workers are done
    }
    printf("Traad %ld: fortsætter, ready_count %d\n", id, ready_count);
    pthread_mutex_unlock(&mutex); // Unlock mutex after waiting
    
   
    
    total=0; 
    for(int i=0;i<N;i++){
        total+=sum[i];
    }
     printf("Traad %ld: efter: %ld\n", id, total);
    //fortsat beregning; ikke en del af opgaven.
    return NULL;
}

int main(int argc, char *argv[]) {                    
  pthread_t t1, t2,t3,t4;
  pthread_create(&t1, NULL, mythread, (void*)0);
  pthread_create(&t2, NULL, mythread, (void*)1); 
  pthread_create(&t3, NULL, mythread, (void*)2); 
  pthread_join(t1, NULL);
  pthread_join(t2, NULL); 
  pthread_join(t3, NULL); 
  return 0;
}