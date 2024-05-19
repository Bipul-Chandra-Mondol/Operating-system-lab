#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *writer(void *wno)
{   
    sem_wait(&wrt);
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n", (*((int *)wno)), cnt);
    sem_post(&wrt);
}

void *reader(void *rno)
{   
    
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    printf("Reader %d: read cnt as %d\n", *((int *)rno), cnt);

    
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   
    int num_readers, num_writers;
    
    printf("Enter the number of readers: ");
    scanf("%d", &num_readers);

    printf("Enter the number of writers: ");
    scanf("%d", &num_writers);

    pthread_t read[num_readers], write[num_writers];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);

    int reader_ids[num_readers];
    for(int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&read[i], NULL, (void *)reader, (void *)&reader_ids[i]);
    }
    int writer_ids[num_writers];
    for(int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&write[i], NULL, (void *)writer, (void *)&writer_ids[i]);
    }

    for(int i = 0; i < num_readers; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < num_writers; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
