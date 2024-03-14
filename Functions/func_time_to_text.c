#include <stdio.h>
/* 
 * Time given in milliseconds is printed 
 * as: hh:mm:ss,xxx in the stream given.
 */
void time_to_text(int time_ms, FILE *fp){
    char time_str[13];
    int hh, mm, ss, xxx;

    hh = time_ms / (3600 * 1000);
    time_ms %= (3600 * 1000);
    mm = time_ms / (60 * 1000);
    time_ms %= (60 * 1000);
    ss = time_ms / 1000;
    xxx = time_ms % 1000;

    sprintf(time_str, "%02d:%02d:%02d,%03d", hh, mm, ss, xxx);

    fprintf(fp, "%s", time_str);
}