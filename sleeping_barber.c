#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 10

sem_t waitingRoom; 
sem_t barberChair; 
sem_t barberPillow; 
sem_t seatBelt; 

int allDone = 0;

void *barber(void *arg) {
    while (!allDone) {
        sem_wait(&barberPillow);
        if (!allDone) {
            printf("Barber is cutting hair.\n");
            sleep(3);
            
            printf("Barber has finished cutting hair.\n");
            sem_post(&seatBelt);
        } else {
            printf("Barber is going home for the day.\n");
        }
    }
    return NULL;
}

void *customer(void *arg) {
    int id = *(int *)arg;

    printf("Customer %d entering the shop.\n", id);

    if (sem_trywait(&waitingRoom) == 0) {
        printf("Customer %d is waiting.\n", id);
        sem_wait(&barberChair);
        sem_post(&waitingRoom);

        printf("Customer %d is waking up the barber.\n", id);
        sem_post(&barberPillow);
        sem_wait(&seatBelt);
        sem_post(&barberChair);
        printf("Customer %d is leaving the shop.\n", id);
    } else {
        printf("Customer %d found no empty waiting chair and is leaving.\n", id);
    }
    free(arg);
    return NULL;
}

int main() {
    pthread_t barberThread;
    pthread_t customerThreads[NUM_CUSTOMERS];
    int i;
    sem_init(&waitingRoom, 0, NUM_CHAIRS);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);
    pthread_create(&barberThread, NULL, barber, NULL);
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        int *customerId = malloc(sizeof(int));
        *customerId = i + 1;
        pthread_create(&customerThreads[i], NULL, customer, customerId);
        sleep(1); 
    }
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customerThreads[i], NULL);
    }
    allDone = 1;
    sem_post(&barberPillow); 
    pthread_join(barberThread, NULL);
    sem_destroy(&waitingRoom);
    sem_destroy(&barberChair);
    sem_destroy(&barberPillow);
    sem_destroy(&seatBelt);

    return 0;
}
