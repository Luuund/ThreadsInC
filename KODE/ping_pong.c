// ping_pong.c
#include <stdio.h>              // printf
#include <pthread.h>            // pthreads, mutex og condition variables

// Global synkronisering
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int turn = 0;  // 0 = ping's tur, 1 = pong's tur

// Ping-funktion: venter på sin tur og udskriver "Ping"
void* ping(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);

        // Vent indtil det er ping's tur
        while (turn != 0)
            pthread_cond_wait(&cond, &mutex);

        printf("Ping\n");
        turn = 1;  // Giv turen til pong

        pthread_cond_signal(&cond);  // Væk pong-tråden
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Pong-funktion: venter på sin tur og udskriver "Pong"
void* pong(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);

        // Vent indtil det er pong's tur
        while (turn != 1)
            pthread_cond_wait(&cond, &mutex);

        printf("Pong\n");
        turn = 0;  // Giv turen til ping

        pthread_cond_signal(&cond);  // Væk ping-tråden
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Start ping og pong tråde
    pthread_create(&t1, NULL, ping, NULL);
    pthread_create(&t2, NULL, pong, NULL);

    // Vent på begge tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- To tråde (ping og pong) skiftes til at udskrive "Ping" og "Pong" 5 gange hver.
- `turn` variablen styrer hvem der har turen.
- `pthread_cond_wait` og `pthread_cond_signal` bruges til at synkronisere udskiftningen.

✅ FORVENTET OUTPUT:
Ping  
Pong  
Ping  
Pong  
... (i alt 10 linjer, skiftevis)

📌 Vigtigt:
- `turn` bruges som signalflag og er beskyttet af mutex.
- `pthread_cond_wait()` slipper mutex’en og genoptager den, når tråden vækkes.
- `pthread_cond_signal()` vækker den næste tråd (modsat `broadcast`, som vækker alle).

🎓 EKSAMENSRELEVANS:
- Eksempel på **koordination mellem tråde**, ikke bare deling af data.
- Brug af **condition variables til tur-skifte** – ofte stillet som opgave!
*/
