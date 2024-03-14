// Combination of reading_srt_file & time_manipulation
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TIME_CHARS 13 // Time is expressed with 12 characters (hh:mm:ss,xxx).
#define SUB_COUNTER_CHARS 6 // characters to store the subtitle counter. Most srt files contain ~1200 subtitles.

int time_to_int(const char*);
void time_to_text(int, FILE*);

int main(void){
    char *sub_file_name = "subfile.srt";
    char *new_sub_file_name = "test_file.srt";
    char init_time[TIME_CHARS], end_time[TIME_CHARS], sub_counter[SUB_COUNTER_CHARS];
    char ch, ch_after_new_line;
    int init_time_ms, end_time_ms;
    int delay = -500;
    FILE *fp1, *fp2;
    
    fp1 = fopen(sub_file_name, "r");
    fp2 = fopen(new_sub_file_name, "w");

    while(!feof(fp1)) {
        fgets(sub_counter, SUB_COUNTER_CHARS, fp1);// sub_counter contains the '\n'.
        if (strcmp(sub_counter, "\n") == 0 || strcmp(sub_counter, "") == 0 || feof(fp1)) break;
        
        fscanf(fp1, "%s --> %s", init_time, end_time); fgetc(fp1);
        init_time_ms = time_to_int(init_time);
        end_time_ms = time_to_int(end_time);

        // Write timestamps on the new file.
        fprintf(fp2, "%s", sub_counter);
        time_to_text(init_time_ms + delay, fp2);
        fprintf(fp2, " --> ");
        time_to_text(end_time_ms + delay, fp2);
        fputc('\n', fp2);
        // Write text on the new file.
        while(1) {
            ch = fgetc(fp1);
            // First '\n'
            if(ch == '\n') {
                fputc(ch, fp2);
                ch_after_new_line = fgetc(fp1);
                // Second '\n' in the row means the end of the subtitle.
                if (ch_after_new_line == '\n' || feof(fp1)) {
                    fputc(ch_after_new_line, fp2);
                    break;
                }
                else {
                    fputc(ch_after_new_line, fp2);
                }
            }
            else {// Copy fp1 to fp2
                fputc(ch, fp2);
            }
        }
    }
    fclose(fp1);
    fclose(fp2);

    return 0;
}

int time_to_int(const char *time_str){
    int hh, mm, ss, xxx, total;

    sscanf(time_str, "%d:%d:%d,%d", &hh, &mm, &ss, &xxx);

    return xxx + 1000 * (ss + mm*60 + hh*3600);
}


void time_to_text(int time_ms, FILE *fp){
    char time_str[TIME_CHARS];
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