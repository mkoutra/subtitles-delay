#include <stdio.h>
/*
 * Reads time with format: hh:mm:ss,xxx
 * and transform it to milliseconds
 */ 
int time_to_int(const char *time_str){
    int hh, mm, ss, xxx, total;

    sscanf(time_str, "%d:%d:%d,%d", &hh, &mm, &ss, &xxx);

    return xxx + 1000 * (ss + mm*60 + hh*3600);
}