#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define MAX_OPTIONS 26

const char* program_names[MAX_OPTIONS] = {
    "06_thread_arguments.exe",
    "07_parallel_sum_barrier.exe",
    "08_thread_safe_logger.exe",
    "09_conditional_wait_agent.exe",
    "10_race_condition_demo.exe",
    "barrier_sync.exe",
    "conditional.exe",
    "countdown_threads.exe",
    "hello_Threads.exe",
    "interleaved_output_safe.exe",
    "multi_consumer.exe",
    "ping_pong.exe",
    "pingpong_mutex_toggle.exe",
    "producer_consumer.exe",
    "read_write_demo.exe",
    "shared_counter_mutex.exe",
    "thread_cascade_sum.exe",
    "thread_chain_dependency.exe",
    "thread_create_loop.exe",
    "thread_pool.exe",
    "thread_return_value.exe",
    "thread_timer_demo.exe",
    "toggle_flag_shared.exe",
    "wait_multiple_threads.exe",
    "write_array.exe",
    "main.exe" // den sidste skal være efter alle andre
};

const char* menu_titles[MAX_OPTIONS] = {
    "06_thread_arguments",
    "07_parallel_sum_barrier",
    "08_thread_safe_logger",
    "09_conditional_wait_agent",
    "10_race_condition_demo",
    "barrier_sync",
    "conditional",
    "countdown_threads",
    "hello_Threads",
    "interleaved_output_safe",
    "multi_consumer",
    "ping_pong",
    "pingpong_mutex_toggle",
    "producer_consumer",
    "read_write_demo",
    "shared_counter_mutex",
    "thread_cascade_sum",
    "thread_chain_dependency",
    "thread_create_loop",
    "thread_pool",
    "thread_return_value",
    "thread_timer_demo",
    "toggle_flag_shared",
    "wait_multiple_threads",
    "write_array",
};

int main() {
    SetConsoleOutputCP(CP_UTF8);  // Sørg for at æøå virker i terminalen
    int choice;
    char again;

    do {
        printf("\nVælg hvilket pthread-program du vil køre:\n");
        for (int i = 0; i < MAX_OPTIONS-1; i++) {
            printf("%2d. %s\n", i + 1, menu_titles[i]);
        }
        printf("\nIndtast valg (1-%d): ", MAX_OPTIONS-1);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > MAX_OPTIONS || choice == 26) {
            printf("Ugyldigt valg.\n");
            while (getchar() != '\n'); // Rens inputbuffer
        } else {
            system(program_names[choice - 1]);  // Kør valgt .exe
        }

        printf("\nVil du køre et andet program? (j/n): ");
        while (getchar() != '\n'); // slug newline
        again = getchar();

    while (again != 'j' && again != 'J' && again != 'n' && again != 'N') {
        printf("Ugyldigt valg. Indtast venligst 'j' eller 'n': ");
        while (getchar() != '\n'); // slug newline
        again = getchar();
    }
    } while (again == 'j' || again == 'J');

    printf("\nFarvel!\n");
    return 0;
}
