#include <stdio.h>          // printf
#include <stdlib.h>         // exit, malloc osv.
#include <pthread.h>        // pthreads til tråde og mutex

int counter = 0; // Delt global variabel, som begge tråde skal øge
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutex til at beskytte counter

// Trådfunktion der øger counter 1.000.000 gange
void* increment(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);   // Lås mutex før adgang til counter
        counter++;                   // Øg counter med 1 (beskyttet af mutex)
        pthread_mutex_unlock(&lock); // Slip mutex igen
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Starter to tråde der kører increment-funktionen
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    // Venter på at begge tråde er færdige
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Udskriver den endelige værdi af counter
    printf("Slutværdi af counter: %d\n", counter);

    return 0;
}

/*
✅ FORVENTET OUTPUT:

Slutværdi af counter: 2000000

🧠 FORKLARING:
- Hver tråd udfører 1.000.000 inkrementeringer => samlet 2.000.000
- Mutex sikrer, at counter++ udføres **atomisk** (én tråd ad gangen)
- UDEN mutex ville der opstå en race condition, og slutværdien kunne være forkert (f.eks. 1734127)

🔐 pthread_mutex_lock / unlock beskytter kritisk sektion, dvs. stedet hvor delt data ændres.

📌 Dette er et klassisk eksempel på **trådsikker inkrementering** med mutex.
*/
