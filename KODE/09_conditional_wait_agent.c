#include <pthread.h>        // pthreads til tråde, mutex og condition variables
#include <stdio.h>          // printf
#include <stdlib.h>         // rand, srand
#include <unistd.h>         // sleep
#include <time.h>           // time (til random seed)

// Globale delte variabler
int bestCost = 100;         // Bedste (laveste) fundne løsning – starter højt
int agentsDone = 0;         // Hvor mange agenter er færdige
int noAgents = 3;           // Antal agent-tråde

// Synkroniseringsmekanismer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;         // Beskytter delt data
pthread_cond_t waitSolution = PTHREAD_COND_INITIALIZER;    // Bruges til at vække ventende tråde

// Kaldes af en agent der har fundet en løsning
void updateCost(int solutionCost) {
    pthread_mutex_lock(&mutex);              // Eksklusiv adgang til bestCost
    if (solutionCost < bestCost) {
        bestCost = solutionCost;             // Opdater hvis bedre løsning fundet
        printf("[Agent] Ny løsning fundet: %d\n", bestCost);
        pthread_cond_broadcast(&waitSolution);  // Væk alle ventende
    }
    pthread_mutex_unlock(&mutex);            // Slip lås
}

// Kaldes af agent når den er færdig med arbejdet
void agentDone() {
    pthread_mutex_lock(&mutex);
    agentsDone++;                            // Øg færdig-tæller
    pthread_cond_broadcast(&waitSolution);   // Væk alle ventende, så de kan genoverveje
    pthread_mutex_unlock(&mutex);
}

// Bruges af klienten til at vente på god nok løsning eller at alle agenter er færdige
int awaitCost(int requiredCost) {
    int foundCost;
    pthread_mutex_lock(&mutex);
    while (bestCost > requiredCost && agentsDone < noAgents) {
        pthread_cond_wait(&waitSolution, &mutex);  // Vent hvis betingelser ikke opfyldt
    }
    foundCost = bestCost;               // Gem det bedste fundne
    pthread_mutex_unlock(&mutex);
    return foundCost;
}

// Trådfunktion for hver agent – forsøger at finde løsninger
void* agent(void* arg) {
    long id = (long)arg;
    int cost = (rand() % 60) + 20;      // Generer en tilfældig cost (20–79)
    sleep(rand() % 3);                  // Simuler søgetid

    printf("Agent %ld fundet løsning med cost: %d\n", id, cost);
    updateCost(cost);
    agentDone();
    return NULL;
}

// Trådfunktion for klienten, der venter på løsning
void* client(void* arg) {
    int required = *(int*)arg;
    printf("[Klient] Venter på løsning <= %d...\n", required);
    int result = awaitCost(required);
    printf("[Klient] Fik løsning: %d\n", result);
    return NULL;
}

int main() {
    srand(time(NULL));  // Initier tilfældighed

    pthread_t agents[noAgents];
    pthread_t klient;
    int målCost = 42;

    // Start agent-tråde
    for (long i = 0; i < noAgents; i++) {
        pthread_create(&agents[i], NULL, agent, (void*)i);
    }

    // Start klient-tråd
    pthread_create(&klient, NULL, client, &målCost);

    // Vent på alle tråde
    for (int i = 0; i < noAgents; i++) {
        pthread_join(agents[i], NULL);
    }
    pthread_join(klient, NULL);

    return 0;
}

/*
🧠 FORKLARING:

Dette program simulerer en typisk søge- eller agent-opsætning med følgende parter:

🔹 Agent-tråde:
- Søger efter løsninger (her: tilfældigt cost mellem 20 og 79)
- Kalder `updateCost()` hvis deres løsning er bedre end nuværende
- Kalder `agentDone()` når de er færdige

🔹 Klient-tråd:
- Kalder `awaitCost()` og venter på løsning ≤ 42
- Vågen igen, hvis en bedre løsning findes ELLER alle agenter er færdige

✅ FORVENTET OUTPUT:
- Agent 0 fundet løsning med cost: 67
- [Agent] Ny løsning fundet: 67
- Agent 2 fundet løsning med cost: 41
- [Agent] Ny løsning fundet: 41
- [Klient] Fik løsning: 41

📌 VIGTIGT:
- Trådsikkerhed opretholdes via mutex
- Venter med `pthread_cond_wait()` og vækker med `pthread_cond_broadcast()`

🎓 RELEVANS:
- Demonstrerer effektiv synkronisering
- Velegnet til eksamensspørgsmål om producer/consumer, tilstandsdeling og ventemekanismer
*/
