// thread_pool_simple.c
#include <stdio.h>          // printf
#include <pthread.h>        // pthreads

#define NUM_THREADS 4       // Antal tråde i vores "trådpool"

// Simuleret arbejdsfunktion som udføres af hver tråd
void* do_work(void* arg) {
    printf("Tråd %ld udfører arbejde\n", (long)arg);  // Udskriver hvilken tråd der arbejder
    return NULL;
}

int main() {
    pthread_t workers[NUM_THREADS];   // Array af tråde

    // Opret tråde og tildel dem "arbejde"
    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&workers[i], NULL, do_work, (void*)i);

    // Vent på at alle tråde afslutter
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(workers[i], NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Dette er en **enkel trådpool** hvor 4 tråde bliver oprettet og hver udfører samme funktion.
- I dette eksempel udfører hver tråd blot én enkelt opgave: udskrift.

✅ FORVENTET OUTPUT (rækkefølge ikke garanteret):
Tråd 0 udfører arbejde  
Tråd 1 udfører arbejde  
Tråd 2 udfører arbejde  
Tråd 3 udfører arbejde

📌 VIGTIGT:
- `pthread_create` tildeler trådene opgaver parallelt.
- `pthread_join` sikrer, at main() venter på, at alle tråde bliver færdige.
- `(void*)i` bruges til at sende tråd-ID’er til hver tråd.

🎓 EKSAMENSRELEVANS:
- Grundstruktur til **trådpool-lignende opgaver**.
- Bruges ofte som base til avanceret tråd-styring med køer eller opgavefordeling.
- Viser hvordan man håndterer mange tråde uden delte data eller mutex.
*/
