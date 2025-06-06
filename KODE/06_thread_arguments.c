#include <pthread.h>       // Indeholder funktioner og typer til tråde (pthread_t, pthread_create osv.)
#include <stdio.h>         // Indeholder printf()

// Definerer en struktur til at holde et interval
typedef struct {
    int start;
    int end;
} Range;

// Trådfunktion der printer tallene fra start til end i et givet Range
void* print_range(void* arg) {
    Range* r = (Range*)arg; // Typecaster argumentet til en Range-pointer
    for (int i = r->start; i <= r->end; i++) {
        printf("%d ", i);   // Udskriver tallet efterfulgt af mellemrum
    }
    printf("\n");           // Til sidst linjeskift
    return NULL;            // Trådfunktionen returnerer NULL (standard for void*)
}

int main() {
    pthread_t t1, t2;           // To tråd-id'er til at holde referencer til trådene

    Range r1 = {1, 5};          // Første interval: 1 til 5
    Range r2 = {6, 10};         // Andet interval: 6 til 10

    // Starter to tråde, hver får deres Range som argument
    pthread_create(&t1, NULL, print_range, &r1);
    pthread_create(&t2, NULL, print_range, &r2);

    // Venter på at begge tråde bliver færdige
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;                   // Programmet afsluttes korrekt
}

/*
FORVENTET OUTPUT (kan variere i rækkefølge, da tråde kører parallelt):

Enten:
1 2 3 4 5 
6 7 8 9 10 

Eller:
6 7 8 9 10 
1 2 3 4 5 

Bemærk: printf fra flere tråde kan i sjældne tilfælde resultere i blandet output,
men i dette lille program vil det typisk optræde i pæne blokke.
*/
