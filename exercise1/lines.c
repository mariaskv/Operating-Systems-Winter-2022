#include <stdio.h>

int n_lines(char* fname){

    int lines = 0;
    FILE* fp = fopen(fname,"r");
    int ch, pre;

    if(fp == NULL){
        perror("Fopen Error");
        return -1;
    }

    while(!feof(fp)){
        ch = fgetc(fp);

        if(pre == '\n' && ch == '\n')
            lines--;

        if(ch == '\n')
            lines++;
        
        pre = ch;
    }

    if(pre == '\n')
        lines--;

    if(fclose(fp) == EOF){
        perror("Fclose Error");
        return -1;
    }

    return lines;

}
