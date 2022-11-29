#include <stdio.h>

int time_to_int(char*);
void time_to_text(int);

int main(void){
    char *time_str = "00:58:08,886";
    int delay = 300; //ms
    int time_in_ms = time_to_int(time_str);

    printf("Time in milliseconds = %d\n", time_in_ms);

    time_to_text(time_in_ms + delay);
    putchar('\n');
    
    return 0;
}

/*
 * Args: char*
 * Returns: int
 * Reads time with format: hh:mm:ss,xxx
 * and transform it to milliseconds
 */ 
int time_to_int(char *time_str){
    int hh, mm, ss, xxx, total;

    sscanf(time_str, "%d:%d:%d,%d", &hh, &mm, &ss, &xxx);

    return xxx + 1000 * (ss + mm*60 + hh*3600);
}

/* 
 * Args: int
 * Returns: void
 * Time given in milliseconds is 
 * printed as: hh:mm:ss,xxx
 */
void time_to_text(int time_ms){
    char time[13];
    int hh, mm, ss, xxx;

    hh = time_ms / (3600 * 1000);
    time_ms %= (3600 * 1000);
    mm = time_ms / (60 * 1000);
    time_ms %= (60 * 1000);
    ss = time_ms / 1000;
    xxx = time_ms % 1000;

    sprintf(time, "%02d:%02d:%02d,%03d", hh, mm, ss, xxx);

    printf("%s", time);
}