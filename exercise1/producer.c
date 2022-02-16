#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int producer(char* txt, char* fname, int pos, int size, int kid){
    FILE* fp = fopen(fname, "r");
    if(fp == NULL){
        perror("Fopen Error");
        return -1;
    }

    for(int i = 0; i < pos; i++){
        fgets(txt, size, fp);
    }

    if(fclose(fp) == EOF){
        perror("Fclose Error");
        return -1;
    }
    return 0;
}