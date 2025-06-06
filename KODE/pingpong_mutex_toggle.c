// pingpong_mutex_toggle.c
#include <stdio.h>              // printf
#include <pthread.h>            // pthreads
#include <unistd.h>             // usleep (mikrosekunder)

// Globalt delt signal: 0 = ping's tur, 1 = pong's tur
int turn = 0;

// Mutex til synkronisering af adgang til 'turn'
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Ping-tråd: tjekker om det er dens tur og udskriver "Ping"
void* ping(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        if (turn == 0) {
            printf("Ping\n");
            turn = 1;  // Giv turen til pong
        }
        pthread_mutex_unlock(&mutex);
        usleep(100000);  // Vent 0,1 sek for at undgå spinning
    }
    return NULL;
}

// Pong-tråd: tjekker om det er dens tur og udskriver "Pong"
void* pong(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        if (turn == 1) {
            printf("Pong\n");
            turn = 0;  // Giv turen til ping
        }
        pthread_mutex_unlock(&mutex);
        usleep(100000);  // Vent 0,1 sek
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Start ping og pong tråde
    pthread_create(&t1, NULL, ping, NULL);
    pthread_create(&t2, NULL, pong, NULL);

    // Vent på at begge tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Programmet kører to tråde: ping og pong.
- De bruger delt variabel `turn` til at afgøre hvem der må skrive.
- En mutex beskytter adgangen til `turn`, så begge tråde ikke skriver samtidigt.

⚠️ FORSKEL FRA CONDITION-VARIANTEN:
- Her bruges ingen `pthread_cond_wait`, så trådene **kører aktivt og tjekker** `turn`.
- Dette kaldes en form for **"busy waiting"** (spinning), dog dæmpet med `usleep`.

✅ FORVENTET OUTPUT:
Ping  
Pong  
Ping  
Pong  
... (5 gange hver)

📌 VIGTIGT:
- Simpel model uden betingelser → lettere at forstå, men ikke så effektiv som `cond`.
- `usleep()` bruges for at forhindre, at CPU'en belastes unødigt ved at trådene spinner hele tiden.

🎓 EKSAMENSRELEVANS:
- God kontrast til en version med `condition variables`.
- Brugbar til at illustrere synkronisering **kun med mutex og variabel**.
*/
