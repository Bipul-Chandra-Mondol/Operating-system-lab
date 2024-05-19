#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

void *dine(void *num);
pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_t chopstick[NUM_CHOPSTICKS];

int main() {
    int i, status_message;
    void *msg;
    for (i = 0; i < NUM_CHOPSTICKS; i++) {
        status_message = pthread_mutex_init(&chopstick[i], NULL);
        if (status_message != 0) {
            printf("\n Mutex initialization failed\n");
            exit(1);
        }
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int *arg = malloc(sizeof(*arg));
        if (arg == NULL) {
            printf("\n Couldn't allocate memory for thread arg.\n");
            exit(1);
        }
        *arg = i;
        status_message = pthread_create(&philosopher[i], NULL, dine, arg);
        if (status_message != 0) {
            printf("\n Thread creation error\n");
            exit(1);
        }
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        status_message = pthread_join(philosopher[i], &msg);
        if (status_message != 0) {
            printf("\n Thread join failed\n");
            exit(1);
        }
    }

    for (i = 0; i < NUM_CHOPSTICKS; i++) {
        status_message = pthread_mutex_destroy(&chopstick[i]);
        if (status_message != 0) {
            printf("\n Mutex Destroyed\n");
            exit(1);
        }
    }

    return 0;
}

void *dine(void *num) {
    int n = *(int *)num;
    free(num);

    printf("\nPhilosopher %d is thinking", n);

    pthread_mutex_lock(&chopstick[n]);

    pthread_mutex_lock(&chopstick[(n + 1) % NUM_CHOPSTICKS]);

    printf("\nPhilosopher %d is eating", n);
    sleep(3);
    pthread_mutex_unlock(&chopstick[n]);

    pthread_mutex_unlock(&chopstick[(n + 1) % NUM_CHOPSTICKS]);
    printf("\nPhilosopher %d finished eating", n);

    return NULL;
}
