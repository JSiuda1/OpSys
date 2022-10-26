#include <stdio.h>
#include <stdint.h>
#include <time.h>

int main() {
    uint64_t i;
    struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 100,
    };

    while (1) {
        i++;
        nanosleep(&req, NULL);
    }

    return 0;
}