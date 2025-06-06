#include <pthread.h>        // pthreads til tråde, mutex og condition variables
#include <stdio.h>          // printf

#define NUM_THREADS 3       // Antal tråde

int sum[NUM_THREADS] = {0}; // Hver tråd skriver sin delsum her
int ready = 0;              // Tæller hvor mange tråde der er klar (barrieresynkronisering)

// Mutex og condition variable til synkronisering
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Trådfunktion
void* thread_fn(void* arg) {
    long id = (long)arg;    // Trådens id (0, 1, 2)

    sum[id] = id + 1;       // Trådens delsum: 1, 2 eller 3

    pthread_mutex_lock(&mutex);
    ready++;                // Øg tælleren for hvor mange tråde der er klar

    if (ready < NUM_THREADS) {
        // Hvis ikke alle tråde er klar endnu, vent på signal
        pthread_cond_wait(&cond, &mutex);
    } else {
        // Sidste tråd: send signal til alle om at fortsætte
        pthread_cond_broadcast(&cond);
    }

    pthread_mutex_unlock(&mutex);   // Slip låsen

    // Når alle tråde er klar, beregner hver den samlede sum
    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        total += sum[i];

    printf("Tråd %ld: samlet sum = %d\n", id, total);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Starter tråde og giver dem deres id (0, 1, 2)
    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, thread_fn, (void*)i);

    // Venter på at alle tråde afslutter
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Hver tråd beregner sin delsum: id+1 (altså 1, 2, 3)
- Ved hjælp af mutex og condition variable sikres det, at alle tråde **venter ved barrieren**, indtil alle er klar
- Herefter beregner de den samlede sum uafhængigt: sum = 1 + 2 + 3 = 6

✅ FORVENTET OUTPUT (rækkefølge varierer):

Tråd 0: samlet sum = 6
Tråd 2: samlet sum = 6
Tråd 1: samlet sum = 6

– Alle tråde skal printe "samlet sum = 6", men rækkefølgen af udskrift kan være forskellig.

📌 Bemærk:
- Uden condition variable kunne nogle tråde forsøge at læse `sum[]`, før de andre tråde har skrevet til det.
- Derfor er `pthread_cond_wait` og `pthread_cond_broadcast` vigtige her som **manuelt implementeret barriere**.
*/
