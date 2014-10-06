#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Globals Vars
pthread_mutex_t lock;
pthread_t tid_push[8];
pthread_t tid_pop[8];

typedef struct buffer {
    int size;
    int start;
    int counter;
    int *element;
} buffer_t;

void create_buffer(buffer_t *buffer, int size) {
    buffer->size = size;
    buffer->start = 0;
    buffer->counter = 0;
    buffer->element = malloc(sizeof(buffer->element)*size);
}

int is_full(buffer_t *buffer) {
    if (buffer->counter == buffer->size) {
        return 1;
    } else {
        return 0;
    }
}

int is_empty(buffer_t *buffer) {
    if (buffer->counter == 0) {
        return 1;
    } else {
        return 0;
    }
}

int push(buffer_t *buffer, int data) {
    int index = 0;

    if (is_full(buffer)) {
        printf("Queue is full\n");
        return 0;
    } else {
        // increasing the buffer counter must be an atomic operation
        pthread_mutex_lock(&lock);
        index = buffer->start + buffer->counter++;
        pthread_mutex_unlock(&lock);

        if (index >= buffer->size) {
            index = 0;
        }
        buffer->element[index] = data;
        return 1;
    }
}

void *pop(buffer_t *buffer) {
    int element;
    if (is_empty(buffer)) {
        printf("Queue is empty\n");
        return NULL;
    } else {
        element = buffer->element[buffer->start];
        buffer->start++;
        pthread_mutex_lock(&lock);
        buffer->counter--;
        pthread_mutex_unlock(&lock);
        if (buffer->start == buffer->size) {
           buffer->start = 0;
        }
       return (int *)element;
    }
}

// from http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

void* enqueue(void *ptr){
    struct buffer *buffer=(struct buffer*)ptr;
    int i = 0;
    int wait_time;
    int rnd;

    srand(time(NULL));

    while( i < 100 ){
        wait_time = rand()%10;
        sleep(wait_time);
        //printf("requesting mutex to add element to queue\n");

        rnd = rand();
        while( 0 == push( buffer, rnd) ){
            sleep(5);
        }
        i++;
    }
}

void* dequeue(void *ptr){
    struct buffer *buffer=(struct buffer*)ptr;
    int i = 0;
    int wait_time, data;

    while ( i < 100 ){
        wait_time = rand()%20;
        sleep(wait_time);
        data = ((int)pop(buffer));
        printf("Dequeue element, %d\n", data);
        i++;
    }
}

int main(int argc, const char *argv[])
{
    int retcode;
    int thread = 0;
    buffer_t buffer;

    // Create mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    // create queue
    create_buffer(&buffer, 10);

    // spawn multiple threads and put something into the queue
    while(thread < 8) {
        retcode = pthread_create(&(tid_push[thread]), NULL, &enqueue, &buffer);
        if(retcode != 0) {
            printf("\ncan't create thread :[%s]", strerror(retcode));
        }
        thread++;
    }

    // spawn multiple threads and read from the queue
    thread = 0;
    while(thread < 8) {
        retcode = pthread_create(&(tid_push[thread]), NULL, &dequeue, &buffer);
        if(retcode != 0) {
            printf("\ncan't create thread :[%s]", strerror(retcode));
        }
        thread++;
    }

    // join threads, delete lock and exit
    for(int i = 0; i < 8; i++) {
        pthread_join(tid_push[i], NULL);
    }
    for(int i = 0; i < 8; i++) {
        pthread_join(tid_pop[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    return 0;
}
