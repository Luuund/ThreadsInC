// thread_countdown_timer.c
#include <pthread.h>    // pthreads
#include <stdio.h>      // printf
#include <windows.h>    // Sleep (på Windows, måles i millisekunder)

// Trådfunktion: tæller ned fra givet antal sekunder
void* countdown(void* arg) {
    int sec = *(int*)arg;  // Hent antal sekunder fra argument

    for (int i = sec; i > 0; i--) {
        printf("Tæller: %d\n", i);   // Udskriv nuværende værdi
        Sleep(1000);                 // Vent 1 sekund (1000 ms)
    }

    printf("Færdig!\n");  // Når nedtællingen er slut
    return NULL;
}

int main() {
    pthread_t t;            // Tråd-ID
    int seconds = 5;        // Hvor mange sekunder vi vil tælle ned fra

    // Opret tråd som tæller ned
    pthread_create(&t, NULL, countdown, &seconds);

    // Vent på tråden afslutter før vi afslutter main
    pthread_join(t, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Tråden tæller ned fra det angivne antal sekunder og venter ét sekund mellem hvert trin.
- `Sleep(1000)` svarer til 1 sekunds ventetid.
- Countdown-argumentet gives som pointer til `int` og derefter derefereres i trådfunktionen.

✅ FORVENTET OUTPUT:
Tæller: 5  
Tæller: 4  
Tæller: 3  
Tæller: 2  
Tæller: 1  
Færdig!

📌 VIGTIGT:
- Brug af `Sleep()` kræver `<windows.h>` og bruges **kun på Windows**.
- Man kunne bruge `usleep(1000000)` på Unix/Linux.

🎓 EKSAMENSRELEVANS:
- Viser hvordan man sender data til en tråd.
- Eksempel på tråd der udfører **tidsbaseret funktionalitet**.
- Brug af `pthread_join()` for at sikre korrekt synkronisering før programmet afsluttes.
*/
