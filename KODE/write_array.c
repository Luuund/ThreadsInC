// write_array.c
#include <stdio.h>              // printf
#include <pthread.h>            // pthreads

int data[10];                   // Delt array som begge tråde skriver til
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Trådfunktion der skriver til array'et
void* writer(void* arg) {
    long id = (long)arg;        // ID'et afgør hvilke tal der skrives

    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);              // Sikrer eksklusiv adgang
        data[i] = id * 10 + i;                   // Skriv f.eks. 10+i, 11+i...
        pthread_mutex_unlock(&mutex);            // Slip lås
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Start to tråde med forskellige ID'er
    pthread_create(&t1, NULL, writer, (void*)1);
    pthread_create(&t2, NULL, writer, (void*)2);

    // Vent på at begge tråde afslutter
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Udskriv det fælles array
    for (int i = 0; i < 10; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }

    return 0;
}

/*
🧠 FORKLARING:
- To tråde skriver til det samme array `data[10]`.
- De bruger mutex for at undgå race conditions, men der er stadig **data-korruption mulig** fordi:
  ▸ Begge tråde skriver til **samme index i array'et**!
  ▸ Mutex beskytter kun én iteration ad gangen, ikke hele arrayet.

✅ FORVENTET OUTPUT:
data[0] = ??  
data[1] = ??  
...
data[9] = ??

👉 Værdierne afhænger af rækkefølgen trådene skriver i – dvs. **usikker/uforudsigelig** udskrift som:
- Tråd 1 skriver `10,11,...,19`
- Tråd 2 skriver `20,21,...,29`
- Men de **overskriver hinanden**, da begge skriver til `data[0]` til `data[9]`.

📌 LØSNING:
- For at undgå dette, skal hver tråd have sit eget **skriveområde** i array'et (f.eks. tråd 1 til indices 0–4, tråd 2 til 5–9)
- Alternativt: brug én tråd ad gangen, eller lav en kø til at fordele arbejdet.

🎓 EKSAMENSRELEVANS:
- Klassisk eksempel på **race condition** på trods af mutex.
- Illustrerer vigtigheden af **logisk adskillelse af data** og ikke kun brug af lås.
- Meget brugbart til spørgsmål om datakonsistens og korrekt trådsamarbejde.
*/
