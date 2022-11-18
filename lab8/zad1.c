#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define THREADS	8

void *hello_scr(void *arg)
{
    int id = *(int*)arg;
    printf("Hello SCR. Written by thread ID %d\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREADS];
    int i;
    for(i = 0; i < THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, hello_scr, (void *) &i);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        sleep(1);
    }

    pthread_exit(NULL);
}
