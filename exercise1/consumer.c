#include <stdio.h>

void consumer(char* txt, int pid, int pos){
    printf("The child with pid: %d, returned the line with id: %d and:%s\n", pid, pos, txt);
}