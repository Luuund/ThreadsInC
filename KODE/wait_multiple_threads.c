// wait_for_all_threads.c
#include <pthread.h>        // pthreads
#include <stdio.h>          // printf
#include <unistd.h>         // sleep

#define N 3                 // Antal arbejder-tråde
int completed = 0;          // Antal færdige tråde
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Trådfunktion – simulerer noget arbejde og signalerer derefter færdig
void* worker(void* arg) {
    sleep(1);   // Simulerer arbejde (1 sek)
    
    pthread_mutex_lock(&mutex);
    completed++;                        // Øg antal færdige tråde
    pthread_cond_signal(&cond);        // Signalér hovedtråden
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[N];

    // Start alle tråde
    for (int i = 0; i < N; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    // Vent indtil alle tråde er færdige
    pthread_mutex_lock(&mutex);
    while (completed < N)              // Hvis ikke alle er færdige, vent
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    printf("Alle tråde færdige (%d/%d)\n", completed, N);

    // Join tråde for at sikre oprydning
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Hovedtråden opretter 3 worker-tråde, som hver simulerer "arbejde" i 1 sekund.
- Når en tråd er færdig, opdaterer den `completed` og sender signal til main().
- Main-tråden venter (med `pthread_cond_wait`) indtil `completed == N`.

✅ FORVENTET OUTPUT:
Alle tråde færdige (3/3)

📌 VIGTIGT:
- Uden mutex og condition variable kunne man risikere race conditions på `completed`.
- `pthread_cond_signal` vækker hovedtråden – men da flere tråde signalerer, er der brug for en while-løkke!
- Det er god praksis at kalde `pthread_join` efter tråde er færdige, så ressourcer bliver ryddet korrekt op.

🎓 EKSAMENSRELEVANS:
- Eksempel på **"vent på alle tråde"** med synkronisering.
- Demonstrerer **brugen af condition variable + mutex** til trådfærdiggørelse.
- Enkelt, men illustrerer mange centrale begreber i trådsynkronisering.
*/
