#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        printf("Hello world!\n");
        sleep(2);
    }
    return 0;
}