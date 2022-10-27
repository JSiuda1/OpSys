#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

// b)
void signal_handler(int sig) {
    printf("Przechwycony sygnal %d\n", sig);
    exit(0);
}

// c)
void signal_handler_c(int sig) {
    printf("Przechwycony sygnal %d\n", sig);
}

// d)

int main() {
    uint64_t i = 0;
    signal(SIGTERM, SIG_IGN); //a)
    // signal(SIGALRM, signal_handler); //b)
    signal(SIGALRM, signal_handler_c); //c)
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    printf("Wstrzymanie odbierania sygnlu\n");
    signal(SIGUSR1, signal_handler_c);
    
    
    while (1) {
        if (i % 10 == 5) {
            printf("Wznowienie odbierania sygnlu\n");
            sigprocmask(SIG_UNBLOCK, &set, NULL);            
        }

        if (i % 10 == 6) {
            printf("Wstrzymanie odbierania sygnlu\n");
            sigprocmask(SIG_BLOCK, &set, NULL);            
        }
        
        sleep(1);
        ++i;
    }

    return 0;
}
// \ls -R