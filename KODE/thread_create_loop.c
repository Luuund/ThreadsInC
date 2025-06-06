// print_thread_ids.c
#include <stdio.h>          // printf
#include <pthread.h>        // pthreads

#define N 5                 // Antal tråde der skal oprettes

// Trådfunktion der printer trådens ID
void* print_id(void* arg) {
    printf("Hej fra tråd %ld\n", (long)arg);  // Udskriver ID'et givet ved oprettelsen
    return NULL;
}

int main() {
    pthread_t threads[N];   // Array til at holde tråd-id'er

    // Opretter N tråde og giver dem et ID (0 til N-1)
    for (long i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, print_id, (void*)i);
    }

    // Venter på at alle tråde afslutter
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/*
🧠 FORKLARING:
- Programmet opretter 5 tråde.
- Hver tråd får et tal mellem 0 og 4 som ID, og printer det ud.
- Bruger `pthread_create` og `pthread_join` til at starte og vente på tråde.

✅ FORVENTET OUTPUT:
Hej fra tråd 0  
Hej fra tråd 1  
Hej fra tråd 2  
Hej fra tråd 3  
Hej fra tråd 4  
(Rækkefølgen kan variere, da tråde kører parallelt)

📌 VIGTIGT:
- Cast af `long` til `(void*)` og tilbage igen er en almindelig teknik til at sende små værdier som argument.
- Ingen mutex er nødvendig her, da `printf` bruges enkeltvis per tråd og ikke manipulerer delt data.

🎓 EKSAMENSRELEVANS:
- Klassisk eksempel på at sende argumenter til tråde.
- Kan bruges som skabelon til tråde med forskellige input.
- Ofte første trin i mere komplekse opgaver (fx summering, synkronisering).
*/
