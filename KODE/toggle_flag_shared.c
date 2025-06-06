// flag_toggle_watch.c
#include <stdio.h>              // printf
#include <pthread.h>            // pthreads
#include <unistd.h>             // sleep (Unix)

int flag = 0;                   // Delt global variabel
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Beskytter adgangen til 'flag'

// Toggler-tråden skifter flag mellem 0 og 1
void* toggler(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);       // Lås inden adgang
        flag = !flag;                     // Skift mellem 0 og 1
        printf("Toggler: flag = %d\n", flag);
        pthread_mutex_unlock(&mutex);     // Slip låsen
        sleep(1);                         // Vent 1 sekund
    }
    return NULL;
}

// Watcher-tråden læser og udskriver værdien af flag
void* watcher(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);       // Lås for sikker aflæsning
        printf("Watcher ser flag = %d\n", flag);
        pthread_mutex_unlock(&mutex);     // Slip låsen
        sleep(1);                         // Vent 1 sekund
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Start toggler- og watcher-tråd
    pthread_create(&t1, NULL, toggler, NULL);
    pthread_create(&t2, NULL, watcher, NULL);

    // Vent på begge tråde
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- To tråde kører samtidig:
  ▸ `toggler()` skifter `flag` mellem 0 og 1 hvert sekund.
  ▸ `watcher()` læser og udskriver flag-værdien.
- Begge bruger mutex for at **undgå race conditions**, når de læser/ændrer den globale variabel `flag`.

✅ FORVENTET OUTPUT (cirka):

Toggler: flag = 1  
Watcher ser flag = 1  
Toggler: flag = 0  
Watcher ser flag = 0  
...

📌 VIGTIGT:
- Uden `pthread_mutex_lock`, kunne `watcher` aflæse en delvis opdateret værdi, eller "misse" ændringer.
- `sleep(1)` gør, at ændringer og aflæsning foregår nogenlunde samtidigt — men rækkefølgen kan variere.

🎓 EKSAMENSRELEVANS:
- Demonstrerer **delt global variabel + mutex-beskyttelse**.
- Bruges ofte i overvågnings- og signaleringseksempler.
- Relevant til spørgsmål om **race conditions og synkronisering**.
*/
