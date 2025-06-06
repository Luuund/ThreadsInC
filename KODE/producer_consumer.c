#include <pthread.h>    // Til tråde, mutex og condition variables
#include <stdio.h>      // Til printf
#include <unistd.h>     // Til usleep()

#define BUFFER_SIZE 5           // Bufferen kan maksimalt indeholde 5 elementer
int buffer[BUFFER_SIZE];        // Delt buffer mellem producer og consumer
int count = 0;                  // Antal elementer i buffer lige nu

// Synkroniseringsmekanismer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;           // Beskytter buffer og count
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;          // Signalerer at buffer ikke er fuld (producer må tilføje)
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;         // Signalerer at buffer ikke er tom (consumer må tage)

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {        // Producer 10 elementer
        pthread_mutex_lock(&mutex);

        // Vent hvis buffer er fuld
        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        // Tilføj et element til buffer
        buffer[count++] = i;
        printf("Producent producerede: %d\n", i);

        // Signalér at buffer ikke længere er tom – consumer kan tage noget
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&mutex);

        usleep(100000);  // Simuler produktionstid (100 ms)
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {        // Forbrug 10 elementer
        pthread_mutex_lock(&mutex);

        // Vent hvis buffer er tom
        while (count == 0)
            pthread_cond_wait(&not_empty, &mutex);

        // Fjern et element fra buffer
        int item = buffer[--count];
        printf("Forbruger forbrugte: %d\n", item);

        // Signalér at buffer ikke længere er fuld – producer kan tilføje noget
        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&mutex);

        usleep(150000);  // Simuler forbrugstid (150 ms)
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Start producer- og consumer-trådene
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Vent på at begge tråde bliver færdige
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Producer genererer værdier og lægger dem i buffer.
- Consumer tager værdier ud fra buffer.
- Begge tråde bruger mutex til at beskytte fælles data.
- Condition variables bruges til at blokere tråde i følgende tilfælde:
    - Producer venter hvis buffer er fuld.
    - Consumer venter hvis buffer er tom.
- Signalerne sørger for, at den anden part kan fortsætte arbejdet.

✅ FORVENTET OUTPUT (rækkefølge kan variere):

Producent producerede: 0  
Forbruger forbrugte: 0  
Producent producerede: 1  
Forbruger forbrugte: 1  
...

– Du vil se en vekselvirkning mellem produktion og forbrug, og at buffer aldrig bliver overfyldt eller underløber.

📌 OBS:
- Dette eksempel viser **klassisk producer/consumer-synkronisering** med condition variables.
- Brug af `usleep()` hjælper med at illustrere asynkron opførsel tydeligt.
*/
