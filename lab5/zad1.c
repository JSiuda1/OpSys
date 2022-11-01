#include <stdio.h>
#include <stdlib.h>

int main() {
    // char *log_name;
    // log_name = getenv("LOGNAME");
    // if (log_name != NULL) {
    //     printf("Hello %s\n", log_name);
    // }
    char * napis = "Witajcie moi mili ...";
    unsigned int index =0;
    for (index; ; index++){
        printf("%c",napis[index]);
    }
    return 0;
}