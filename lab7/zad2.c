#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]){
    int memory_file_fd;
    struct stat memory_file_stat;

    memory_file_fd = open("mem2", O_RDWR);
    fstat(memory_file_fd, &memory_file_stat);

    char *memory_ptr = mmap(
        NULL,
        memory_file_stat.st_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        memory_file_fd,
        0
    );

    if(memory_ptr == MAP_FAILED){
        printf("Mapping Failed - file empty\n");
        return 1;
    }

    pid_t fork_id;
    fork_id = fork();
    if (fork_id == -1) {
        printf("Fork error\n");
        return -1;
    }

    if (fork_id == 0) {
        execlp("display", "display", "-update", "1", "mem2", (char*)NULL);
    }

    char file_name[20] = {0};
    while(1) {
        printf("\nInsert file name\n");
        scanf("%s", file_name);

        int file_descriptor = open(file_name, O_RDWR);
        if(file_descriptor < 0){
            perror("Could not open file");
            return -1;
        }

        struct stat file_stat;
        if (fstat(file_descriptor, &file_stat) < 0) {
            perror("Could not open file");
            return -1;
        }

        char *file_memory_ptr = mmap(
            NULL,
            file_stat.st_size,
            PROT_READ|PROT_WRITE,
            MAP_PRIVATE,
            file_descriptor,
            0
        );

        if(file_memory_ptr == MAP_FAILED){
            perror("Mapping Failed\n");
            return 1;
        }
        printf("File size %ld\n", file_stat.st_size);
        printf("Mem file size %ld \n", memory_file_stat.st_size);
        ftruncate(memory_file_fd, file_stat.st_size);

        munmap(memory_ptr, memory_file_stat.st_size);

        memory_ptr = mmap(
            NULL,
            file_stat.st_size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            memory_file_fd,
            0
        );


        fstat(memory_file_fd, &memory_file_stat);
        printf("New size %ld", memory_file_stat.st_size);
        memcpy(memory_ptr, file_memory_ptr, memory_file_stat.st_size);

        munmap(file_memory_ptr, file_stat.st_size);
        close(file_descriptor);
    }

    munmap(memory_ptr, memory_file_stat.st_size);
    close(memory_file_fd);
    return 0;
}