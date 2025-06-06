// interleaved_output_safe.c
#include <pthread.h>        // pthreads til tråde og mutex
#include <stdio.h>          // printf

// Global mutex til at sikre trådsikker udskrift
pthread_mutex_t output_lock = PTHREAD_MUTEX_INITIALIZER;

// Trådfunktion som printer tekst – trådsikkert
void* safe_print(void* arg) {
    pthread_mutex_lock(&output_lock);   // Lås mutex før udskrift
    printf("Udskrift fra tråd: %s\n", (char*)arg);
    pthread_mutex_unlock(&output_lock); // Slip mutex efter udskrift
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Starter to tråde, med hver sin tekst
    pthread_create(&t1, NULL, safe_print, "A");
    pthread_create(&t2, NULL, safe_print, "B");

    // Venter på at begge tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

/*
🧠 FORKLARING:
- Tråde `t1` og `t2` kører samtidig og kalder `printf`.
- Uden mutex kunne udskrifterne blande sig (fx: "Udskrift fra tråd: AUdskrift fra tråd: B").
- Mutexen `output_lock` sikrer, at kun én tråd ad gangen kan udskrive – dermed **trådsikker udskrift**.

✅ FORVENTET OUTPUT (rækkefølgen kan variere, men aldrig blandes):

Udskrift fra tråd: A  
Udskrift fra tråd: B

eller:

Udskrift fra tråd: B  
Udskrift fra tråd: A

🔒 `pthread_mutex_lock` og `pthread_mutex_unlock` bruges til at **beskytte printf**, som ikke er trådsikker alene.
*/
