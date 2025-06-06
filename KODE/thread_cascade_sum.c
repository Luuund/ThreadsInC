// thread_cascade_sum.c
#include <pthread.h>        // pthreads
#include <stdio.h>          // printf

#define N 5                 // Antal værdier/tråde

int result = 0;             // Global sum-variabel (deles mellem tråde)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutex til at beskytte 'result'

// Trådfunktion: tilføjer én værdi til den fælles sum
void* add_number(void* arg) {
    int val = *(int*)arg;

    pthread_mutex_lock(&lock);   // Beskyt adgang til 'result'
    result += val;
    pthread_mutex_unlock(&lock); // Slip låsen, så andre tråde kan fortsætte

    return NULL;
}

int main() {
    pthread_t threads[N];
    int vals[N] = {1, 2, 3, 4, 5};   // De værdier der skal lægges sammen

    // Start én tråd pr. værdi
    for (int i = 0; i < N; i++)
        pthread_create(&threads[i], NULL, add_number, &vals[i]);

    // Vent på at alle tråde afslutter
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    printf("Summen af alle værdier: %d\n", result);  // Skal være 15

    return 0;
}

/*
🧠 FORKLARING:
- Hver tråd får én værdi (1–5) og lægger den til den globale variabel `result`.
- Mutexen sikrer, at kun én tråd ad gangen kan opdatere summen → undgår race condition.

✅ FORVENTET OUTPUT:
Summen af alle værdier: 15

📌 VIGTIGT:
- `result` er delt mellem tråde og skal derfor **beskyttes med mutex**.
- Alternativt kunne man bruge `atomic int` (hvis understøttet), men `mutex` er mere fleksibelt og eksamensrelevant.

🎓 EKSAMENSRELEVANS:
- Eksempel på **delte ressourcer + mutex-beskyttelse**.
- Almindeligt mønster i trådprogrammering: parallel akkumulation.
*/
