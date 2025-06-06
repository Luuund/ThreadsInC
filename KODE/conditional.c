#include <stdio.h>          // printf
#include <stdlib.h>         // exit, malloc osv.
#include <pthread.h>        // pthreads til tråde, mutex og condition variables
#include <Windows.h>

// Globale synkroniseringsobjekter
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int ready = 0;  // Signalfelt: 0 = ikke klar, 1 = klar (bruges til at kontrollere hvornår tråde må køre videre)

// Arbejder-tråde venter på signal
void* worker(void* arg) {
    pthread_mutex_lock(&mutex);

    // Vent indtil 'ready' bliver sat til 1
    while (!ready) {
        pthread_cond_wait(&cond, &mutex); // Venter på condition variable, slipper mutex midlertidigt
    }

    // Når condition er opfyldt, fortsæt
    printf("Tråd %ld kører efter signal\n", (long)arg);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

// Controller-tråd der "vækker" arbejderne
void* controller(void* arg) {
    Sleep(1000);  // Simulerer noget forudgående arbejde (fx initialisering)

    pthread_mutex_lock(&mutex);
    ready = 1; // Sæt ready-flag til sand – trådene må nu fortsætte
    pthread_cond_broadcast(&cond);  // Vækker alle tråde, der venter på cond
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2, control;

    // Opretter to worker-tråde
    pthread_create(&t1, NULL, worker, (void*)1);
    pthread_create(&t2, NULL, worker, (void*)2);

    // Opretter en controller-tråd der sender signal
    pthread_create(&control, NULL, controller, NULL);

    // Venter på alle tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(control, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- t1 og t2 (worker-tråde) starter og venter straks på et signal via condition variable.
- controller-tråden sover i 1 sekund og sætter derefter 'ready = 1' og sender et 'broadcast' signal.
- Når signalet kommer, vågner begge tråde og skriver deres besked.

✅ FORVENTET OUTPUT:

Tråd 1 kører efter signal  
Tråd 2 kører efter signal

🔁 Rækkefølgen kan bytte rundt afhængig af hvilken tråd der vågner først.

📌 Vigtigt:
- `pthread_cond_wait` slipper mutex’en og genoptager den automatisk ved opvågning.
- `pthread_cond_broadcast` vækker **alle** ventende tråde (modsat `pthread_cond_signal`, der kun vækker én).
*/
