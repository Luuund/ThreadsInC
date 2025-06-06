#include <pthread.h>        // pthreads til tråde
#include <stdio.h>          // printf

int counter = 0;            // Global variabel der ændres af flere tråde (ikke beskyttet!)

void* unsafe_increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;          // Ikke trådsikker – her opstår race condition
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;       // To tråde oprettes

    // Starter to tråde, som begge forsøger at tælle op til 100000
    pthread_create(&t1, NULL, unsafe_increment, NULL);
    pthread_create(&t2, NULL, unsafe_increment, NULL);

    // Venter på at begge tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Printer resultatet
    printf("Uden mutex, counter = %d (bør være 200000)\n", counter);
    return 0;
}

/*
🧠 FORKLARING:
- Hver tråd udfører 100000 increment-operationer på den fælles globale variabel `counter`.
- counter++ er **ikke en atomisk operation** – den læser, øger og skriver – og disse trin kan blive blandet af tråde.

❗ Derfor: UDEN mutex risikerer man race conditions, hvor flere tråde forsøger at opdatere `counter` samtidig.
    -> Nogle opdateringer bliver overskrevet eller glemt.

✅ Forventet output (eksempel):

Uden mutex, counter = 178932 (bør være 200000)

– Det præcise tal varierer fra kørsel til kørsel, men det er altid ≤ 200000.

🛠️ LØSNING:
- For at gøre dette sikkert, bør man bruge `pthread_mutex_t` til at låse `counter++`.
*/
