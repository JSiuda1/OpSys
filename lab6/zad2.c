#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pipfd[2] = {0, 0};
    char data;
    pid_t fork_id;

    if (argc != 2) {
        printf("Invalid arguments number\n");
        return -1;
    }

    if (pipe(pipfd) == -1) {
        printf("Pipe error\n");
        return -1;
    }

    fork_id = fork();
    if (fork_id == -1) {
        printf("Fork error\n");
        return -1;
    }

    if (fork_id == 0) { //child process
        close(pipfd[1]); //close child write pipe
        close(0); // close stdin (zero descirptor)
        dup(pipfd[0]);  //redirect pipe to stdin (zero descriptor)
        execlp("display", "display", (char*)NULL);
    } else { //parent process
        FILE *fptr;
        fptr = fopen(argv[1], "r");
        if (fptr == NULL) {
            return -1;
        }

        close(pipfd[0]); //close parent read pipe
        while (feof(fptr) == 0) {
            data = fgetc(fptr);
            write(pipfd[1], &data, sizeof(data));
        }
        fclose(fptr);
    }

    return 0;
}