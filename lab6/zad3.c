#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("Invalid arguments number\n");
        return -1;
    }

    char fifo_path[] = "fifo";
    int file_descriptor;
    mkfifo(fifo_path, 0666);
    file_descriptor = open(fifo_path, O_WRONLY);
    FILE *fptr;
    for (int i = 1; i < argc; ++i) {
        fptr = fopen(argv[i], "r");
        if (fptr == NULL) {
            perror("Fopen error");
            return -1;
        }
        sleep(1);
        char data;
        while (feof(fptr) == 0) {
            data = fgetc(fptr);
            if (data != EOF) {
                write(file_descriptor, &data, sizeof(data));
            }
        }
        fclose(fptr);
    }
    close(file_descriptor);

    return 0;
}