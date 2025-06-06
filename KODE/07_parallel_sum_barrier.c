#include <pthread.h>        // pthreads til tråde, mutex og condition variables
#include <stdio.h>          // printf

const int N = 3;            // Antal elementer pr. tråd (pr. blok)
long arr[9] = {1,2,3, 4,5,6, 7,8,9}; // Et array på 9 elementer, opdeles i 3 blokke
long sum[3] = {0,0,0};      // Her lagrer hver tråd sin delsum

// Synkroniseringsmekanismer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;      // Lås til adgangskontrol
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;         // Condition variable til barrieresynkronisering
int ready_count = 0;        // Tæller hvor mange tråde der er klar

// Trådfunktion
void* mythread(void* arg) {
    long id = (long)arg;    // Hver tråd får sit id (0, 1, 2)
    long total = 0;

    // Udregner delsum af sin blok i arr[]
    for (int i = 0; i < N; i++)
        total += arr[id * N + i];
    sum[id] = total;

    printf("Tråd %ld før: %ld\n", id, total); // Udskriver sin delsum

    // Barriere: alle tråde skal vente her til alle har lavet deres delsum
    pthread_mutex_lock(&mutex);
    ready_count++;          // Øger antallet af færdige tråde
    if (ready_count < 3) {
        // Hvis ikke alle tråde er færdige endnu, vent
        pthread_cond_wait(&cond, &mutex);
    } else {
        // Sidste tråd: væk alle andre
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);

    // Efter barrieren: alle tråde beregner den samlede sum
    total = 0;
    for (int i = 0; i < N; i++)
        total += sum[i];

    printf("Tråd %ld efter: %ld\n", id, total); // Udskriver samlet sum
    return NULL;
}

int main() {
    pthread_t threads[3];   // Holder ID’er på de tre tråde

    // Starter tråde med id 0, 1 og 2
    for (long i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, mythread, (void*)i);

    // Venter på alle tråde er færdige
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

/*
FORVENTET OUTPUT (rækkefølgen kan variere pga. samtidighed):

Tråd 0 før: 6
Tråd 1 før: 15
Tråd 2 før: 24
Tråd 0 efter: 45
Tråd 1 efter: 45
Tråd 2 efter: 45

Forklaring:
- Tråd 0 lægger sammen: 1+2+3 = 6
- Tråd 1 lægger sammen: 4+5+6 = 15
- Tråd 2 lægger sammen: 7+8+9 = 24
- Efter barrieren summerer alle: 6+15+24 = 45

Rækkefølgen af "før" og "efter" linjerne kan være forskellig, men de vil altid have korrekt værdi.
*/
