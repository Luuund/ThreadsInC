// countdown_threads.c
#include <stdio.h>          // printf
#include <pthread.h>        // pthreads til tråde
#include <windows.h>        // Sleep (på Windows, millisekunder)
#include <stdint.h>  // long type til tråd-id


// Trådfunktion: tæller ned fra 5 med 0,5 sek. mellemrum
void* countdown(void* arg) {
    for (int i = 5; i >= 1; i--) {
       printf("Tråd %ld: %d\n", (long)(intptr_t)arg, i); // Udskriv tråd-id og tal
        Sleep(500);  // Vent 0,5 sekunder (500 ms)
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Starter to tråde, som hver tæller ned fra 5
    pthread_create(&t1, NULL, countdown, (void*)1);
    pthread_create(&t2, NULL, countdown, (void*)2);

    // Venter på at begge tråde afslutter før programmet afsluttes
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Programmet starter to tråde, der hver tæller ned fra 5.
- Begge tråde sover 0,5 sek. mellem hvert tal via Sleep().
- Udskriften blandes ofte, da trådene kører parallelt.

✅ FORVENTET OUTPUT (kan variere i rækkefølge):

Tråd 1: 5  
Tråd 2: 5  
Tråd 1: 4  
Tråd 2: 4  
...

🔁 Trådene kører parallelt, så rækkefølgen kan være skiftende men følger nedtællingsmønster.

📌 Bemærk:
- `Sleep()` bruges i millisekunder, derfor 500 = 0,5 sek.
- Brug `(long)arg` til at få ID'et som heltal i `printf`.
*/
