// multi_consumer.c
#include <stdio.h>          // printf
#include <pthread.h>        // pthreads, mutex og condition variables

// Global buffer og synkroniseringsobjekter
int buffer = -1;                                // -1 = tom buffer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// PRODUCENT-tråd: skriver en værdi til buffer og vækker alle forbrugere
void* producer(void* arg) {
    pthread_mutex_lock(&mutex);
    buffer = 42;                                // Sætter en værdi i den delte buffer
    printf("Producer har sat buffer = %d\n", buffer);
    pthread_cond_broadcast(&cond);              // Vækker ALLE tråde, der venter på cond
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// FORBRUGER-tråde: venter på at buffer er klar og læser den
void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);
    while (buffer == -1)                         // Hvis buffer er tom, vent
        pthread_cond_wait(&cond, &mutex);        // Vent og slip mutex imens
    printf("Consumer %ld læste: %d\n", (long)arg, buffer);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t prod, cons[3];

    // Start producent
    pthread_create(&prod, NULL, producer, NULL);

    // Start 3 forbrugere
    for (long i = 0; i < 3; i++)
        pthread_create(&cons[i], NULL, consumer, (void*)i);

    // Vent på at producent og alle forbrugere afslutter
    pthread_join(prod, NULL);
    for (int i = 0; i < 3; i++)
        pthread_join(cons[i], NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Programmet demonstrerer én producent og flere forbrugere.
- Forbrugerne venter passivt på signal, hvis `buffer == -1`.
- Producenten udfylder bufferen og sender et **broadcast-signal**, der vækker alle.

✅ FORVENTET OUTPUT (rækkefølgen kan variere, men ALLE consumers læser 42):

Producer har sat buffer = 42  
Consumer 0 læste: 42  
Consumer 1 læste: 42  
Consumer 2 læste: 42

📌 Vigtigt:
- `pthread_cond_broadcast` vækker **alle tråde**, modsat `pthread_cond_signal`, som kun vækker én.
- `pthread_cond_wait` slipper mutex midlertidigt og genoptager den, når tråden vækkes.
- `buffer` er delt data og skal derfor beskyttes med mutex.

🎓 Relevans til eksamen:
- Brug af condition variables til at synkronisere adgang til delt data.
- Eksempel på “forbrugere der venter på at noget bliver produceret”.
*/
