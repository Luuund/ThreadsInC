#include <pthread.h>    // POSIX Threads
#include <stdio.h>      // printf

int shared = 0;         // Delt variabel, som både læses og skrives
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;  // Read-Write-lås

// Writer-tråd skriver til shared
void* writer(void* arg) {
    pthread_rwlock_wrlock(&rwlock);              // Eksklusiv lås – ingen andre må læse eller skrive samtidig
    shared += 1;
    printf("Writer opdaterede shared til %d\n", shared);
    pthread_rwlock_unlock(&rwlock);              // Lås slippes – læsere og evt. skrivere kan nu fortsætte
    return NULL;
}

// Reader-tråde læser fra shared
void* reader(void* arg) {
    pthread_rwlock_rdlock(&rwlock);              // Shared lås – flere læsere må læse samtidig
    printf("Reader så shared = %d\n", shared);
    pthread_rwlock_unlock(&rwlock);              // Lås slippes
    return NULL;
}

int main() {
    pthread_t r1, r2, w1;

    // Opret tråde: reader, writer, reader
    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&r2, NULL, reader, NULL);

    // Vent på alle tråde
    pthread_join(r1, NULL);
    pthread_join(w1, NULL);
    pthread_join(r2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Der er to typer adgang:
    - `pthread_rwlock_rdlock`: flere læsere kan læse samtidig
    - `pthread_rwlock_wrlock`: kun én skribent ad gangen – eksklusiv adgang
- Her bruges 2 readers og 1 writer. Rækkefølgen af deres udskrift kan variere.

✅ FORVENTET OUTPUT (en mulig variant):

Reader så shared = 0  
Writer opdaterede shared til 1  
Reader så shared = 1

Andre kombinationer er mulige, afhængigt af OS-trådscheduling.

📌 Hvis begge læsere når frem før skriveren, kan output blive:
Reader så shared = 0  
Reader så shared = 0  
Writer opdaterede shared til 1

🔒 RW-locks er nyttige i scenarier hvor der er **flere læsere og færre skrivere**, og man vil tillade parallel læsning men beskytte mod race ved skrivning.
*/
