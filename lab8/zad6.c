#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM	30000

void *monte_carlo(void *pi) {

    const size_t total_points = 100;
    size_t points_in_circle = 0;
    double p_x;
    double p_y;
    double length_square;

    // Promien kola to 1
    for (int i = 0; i < total_points; ++i) {
        p_x = drand48();
        p_y = drand48();
        length_square = p_x * p_x + p_y *p_y;
        if (length_square <= 1) {
            points_in_circle++;
        }
    }

    double *pi_est = (double*) pi;
    *pi_est = (float)points_in_circle / (float)total_points * 4;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_NUM] = {0};
    double pi[THREAD_NUM] = {0};


   for(int i = 0; i < THREAD_NUM; i++){
        int rc = pthread_create(&threads[i], NULL, monte_carlo, (void *)&pi[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    sleep(1);

    double pi_sum = 0;
    for (int i = 0; i < THREAD_NUM; ++i) {
        printf("Thread %d: Pi: %f\n", i, pi[i]);
        pi_sum += pi[i];
    }

    printf("Estimated pi: %f\n", pi_sum/THREAD_NUM);
    pthread_exit(NULL);

}