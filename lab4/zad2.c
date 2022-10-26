#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

void sig_handler_b(int sig) {
    printf("Received signal %d\n", sig);
    exit(0);
}

void sig_handler_c(int sig) {
    printf("Received signal %d\n", sig);
}

int main() {
    uint64_t i;
    struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 100,
    };

    // // a) całkowite ignorowanie sygnału
    // if (signal(SIGALRM, SIG_IGN) == SIG_ERR){
    //     printf("Signal error!\n");
    //     return 0;
    // }

    // b) wyświetlanie komunikatu i zakończenie pracy sygnału
    // if (signal(SIGALRM, sig_handler_b) == SIG_ERR) {
    //     printf("Set signal handler error!\n");
    //     return 0;
    // }

    // c) wyświetlenie komunikatu i powrót do wykonywanego programu
    if (signal(SIGALRM, sig_handler_c) == SIG_ERR) {
        printf("Set signal handler error!\n");
        return 0;
    }

    // signal(SIGALRM, SIG_IGN);
    // printf("Odbieranie sygnału wstrzymane\n");
    while (1) {
        // d) wstrzymywanie odbierania sygnału
        // if (i % 1000 == 500){
        //     signal(SIGALRM, SIG_DFL);
        //     printf("Odbieranie sygnału wznowione\n");
        // }

        // if (i % 1000 == 600) {
        //     signal(SIGALRM, SIG_IGN);
        //     printf("Odbieranie sygnału wstrzymane\n");
        // }

        i++;
        nanosleep(&req, NULL);
    }

    return 0;
}