#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        perror("Invalid number of arguments\n");
        return -1;
    }

    int file_descriptor = open(argv[1], O_RDWR);
    if(file_descriptor < 0){
        perror("Could not open file");
        return -1;
    }

    size_t memory_size = 10;
    char *memory_ptr = mmap(NULL , memory_size, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0,0);
    if(memory_ptr == MAP_FAILED){
        perror("Mapping Failed - file empty :C \n");
        return 1;
    }

    for (int i = 0; i < memory_size; i++) {
        memory_ptr[i] = 48 + i;
    }

    write(file_descriptor, memory_ptr, memory_size);
    close(file_descriptor);

    int res = munmap(memory_ptr, memory_size);
    if (res < 0) {
        perror("UnMap failed\n");
    }
    return 0;
}