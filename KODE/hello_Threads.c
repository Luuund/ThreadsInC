#include <pthread.h>  // Inkluderer POSIX Threads-biblioteket
#include <stdio.h>    // Til printf og anden standard I/O

// Denne funktion bliver udført af den nye tråd
void* thread_func(void* arg) {
    // Udskriv besked fra tråden
    printf("Hello from thread!\n");
    return NULL;  // Trådfunktion skal returnere void*
}

int main() {
    pthread_t thread;  // Variabel til at holde ID’et på den nye tråd

    // Opretter en ny tråd:
    // - &thread: pointer til hvor tråd-ID’et skal gemmes
    // - NULL: standard attributter for tråden
    // - thread_func: funktionen som tråden skal køre
    // - NULL: argument der sendes til thread_func (her ingen)
    pthread_create(&thread, NULL, thread_func, NULL);

    // Vent på at den nye tråd bliver færdig
    // Dette sikrer, at "Hello from thread!" bliver udskrevet
    pthread_join(thread, NULL);

    // Udskriv besked fra hovedtråden, efter at den anden tråd er afsluttet
    printf("Thread joined.\n");

    return 0;  // Afslutter programmet
}

/*
✅ FORVENTET OUTPUT:

Hello from thread!
Thread joined.

🧠 FORKLARING:
- Tråden udskriver sin besked, og main-tråden venter med at afslutte til tråden er færdig.
- Dette er et grundlæggende eksempel på hvordan man starter og venter på tråde i pthreads.

🔒 Uden pthread_join kunne hovedtråden afslutte før den nye tråd nåede at udskrive sin besked.
*/
