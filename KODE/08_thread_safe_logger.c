#include <pthread.h>        // pthreads til tråde og mutex
#include <stdio.h>          // printf

// Mutex bruges til at sikre, at kun én tråd logger ad gangen
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Trådfunktion der skriver en besked til skærmen
void* log_message(void* arg) {
    pthread_mutex_lock(&log_mutex);     // Lås loggen, så ingen andre tråde kan skrive samtidig
    printf("Log: %s\n", (char*)arg);    // Udskriv besked fra argument
    pthread_mutex_unlock(&log_mutex);   // Lås op igen, så andre tråde kan logge
    return NULL;
}

int main() {
    pthread_t t1, t2;   // Tråd-ID'er til de to log-tråde

    // Starter to tråde, hver med en unik besked
    pthread_create(&t1, NULL, log_message, "Tråd 1 besked");
    pthread_create(&t2, NULL, log_message, "Tråd 2 besked");

    // Venter på at begge tråde bliver færdige
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
FORVENTET OUTPUT:

Log: Tråd 1 besked
Log: Tråd 2 besked

Eller:

Log: Tråd 2 besked
Log: Tråd 1 besked

Forklaring:
- Mutex sikrer, at kun én tråd skriver ad gangen, så output bliver **rent og ublandet**.
- Rækkefølgen afhænger af hvilken tråd der kører først.
- Uden mutex kunne output se rodet ud, fx "Log: Tråd Log: Tråd 21  beskedbesked"
*/
