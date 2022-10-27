#include <stdio.h>
#include <stdint.h>
#include <time.h>

int main() {
    uint64_t i = 0;
    struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 1000,
    };
    
    while (1) {
        nanosleep(&req, NULL);
        ++i;
    }
    return 0;
}