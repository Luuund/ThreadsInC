// sequential_stage_threads.c
#include <stdio.h>              // printf
#include <pthread.h>            // pthreads, mutex og condition variables

// Global synkronisering
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conds[3];        // En condition variable pr. "trin"
int stage = 0;                  // Hvilket trin er aktivt (starter med 0)

// Trådfunktion: venter på sin tur og kører kun i rækkefølge
void* run_stage(void* arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);

    // Vent på at 'stage' matcher denne tråds ID
    while (stage != id)
        pthread_cond_wait(&conds[id], &mutex);

    // Det er denne tråds tur
    printf("Trin %ld kører\n", id);

    stage++;  // Næste trin aktiveres
    pthread_cond_signal(&conds[stage]);  // Væk næste tråd (hvis nogen)

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[3];

    // Initialiser alle condition variables
    for (int i = 0; i < 3; i++)
        pthread_cond_init(&conds[i], NULL);

    // Opret 3 tråde, hver med et trin-id
    for (long i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, run_stage, (void*)i);

    // Spark gang i første trin
    pthread_cond_signal(&conds[0]);

    // Vent på at alle tråde afslutter
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    // Ryd op efter condition variables
    for (int i = 0; i < 3; i++)
        pthread_cond_destroy(&conds[i]);

    return 0;
}

/*
🧠 FORKLARING:
- 3 tråde bliver oprettet, men kun én må køre ad gangen – i rækkefølge.
- Trin 0 starter først, så trin 1, derefter trin 2.
- Hver tråd venter på sin egen condition variable.
- Når én tråd er færdig, vækker den den næste ved at signalere dens cond.

✅ FORVENTET OUTPUT:
Trin 0 kører  
Trin 1 kører  
Trin 2 kører

📌 VIGTIGT:
- `stage` holder styr på hvilken tråd der må køre.
- `pthread_cond_signal` bruges til at vække den næste i rækken.
- Trinene kører altid i rækkefølge – selvom trådene bliver oprettet samtidigt.

🎓 EKSAMENSRELEVANS:
- Viser **sekventiel synkronisering** mellem flere tråde.
- Demonstrerer **enkel trådkoordination** med flere condition variables.
- Eksempel på hvordan man implementerer afhængigheder mellem tråde.
*/
