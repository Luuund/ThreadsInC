#include <pthread.h>    // Inkluderer pthreads biblioteket – nødvendigt for at arbejde med tråde
#include <stdio.h>      // Indeholder printf() til udskrift
#include <stdlib.h>     // Indeholder malloc() og free() til dynamisk hukommelsesallokering

// Trådfunktion til at beregne kvadratet af et tal
void* compute_square(void* arg) {
    // Typecast 'arg' (void pointer) til en int pointer og hent værdien
    int val = *(int*)arg;
    
    // Allokerer dynamisk hukommelse til et int resultat
    int* result = malloc(sizeof(int));
    if (result == NULL) {  // Simpel fejlhåndtering
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    
    // Beregner kvadratet af det indkomne tal og gemmer det i den allokerede hukommelse
    *result = val * val;
    
    // Returnerer pointeren til resultatet (dette vil være tilgængeligt i main via pthread_join)
    return result;
}

int main() {
    pthread_t t;         // Variabel til at holde trådens ID (en reference til den oprettede tråd)
    int input = 4;       // Værdien, der skal kvadreres
    
    // Opretter en ny tråd. Denne kører 'compute_square' funktionen med 'input' som argument.
    // &t: hvor tråd-ID'et gemmes
    // NULL: standard tråd-attributter
    // compute_square: funktionen, der skal køres i tråden
    // &input: addressen til input, som sendes til compute_square
    pthread_create(&t, NULL, compute_square, &input);
    
    void* res;           // Variabel til at modtage resultatet fra tråden
    // Vent på at tråden afslutter. Når den er færdig, returneres værdien (pointer til int) i 'res'
    pthread_join(t, &res);
    
    // Udskriv resultatet: 'input' og kvadratet af input. 
    // Da 'res' er en pointer til det allokerede int, skal vi typecaste den for at få værdien.
    printf("Kvadratet af %d er %d\n", input, *(int*)res);
    
    free(res);           // Frigiver den dynamisk allokerede hukommelse til 'result'
    return 0;            // Afslutter programmet
}

/*
✅ FORVENTET OUTPUT:
Kvadratet af 4 er 16

🧠 FORKLARING:
1. Main tråden sætter input til 4 og starter en ny tråd, der kører compute_square(&input).
2. compute_square modtager input, beregner 4 * 4, og allokerer hukommelse til resultatet 16.
3. compute_square returnerer en pointer til det allokerede resultat.
4. main tråden venter med pthread_join, henter resultatet, og udskriver "Kvadratet af 4 er 16".
5. Til sidst frigives den allokerede hukommelse med free().

Denne teknik med at returnere en pointer er nyttig, når man ønsker at overføre data fra en tråd til main-tråden.
*/
