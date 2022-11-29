/*  
 * Subtitle files are of the form:
 * 
 * 623
 * 00:50:05,583 --> 00:50:10,000
 * Ριψοκίνδυνοι άντρες που στέλνετε
 * όταν δεν μπορείτε να στείλετε άλλον.
 * 
 * 624
 * 00:50:10,083 --> 00:50:12,916
 * Οι Γκρίζοι. Ιδέα του προκατόχου μου.
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TIME_CHARS 13 // Time is expressed with 12 characters (hh:mm:ss,xxx).
#define SUB_COUNTER_CHARS 6 // characters to store the subtitle counter. Most srt files contain ~1200 subtitles.

int main(void){
    char *sub_file_name = "subfile.srt";
    char *new_sub_file_name = "test_file.txt";
    char init_time[TIME_CHARS], end_time[TIME_CHARS], sub_counter[SUB_COUNTER_CHARS];
    FILE *fp1, *fp2;
    
    fp1 = fopen(sub_file_name, "r");
    fp2 = fopen(new_sub_file_name, "w");

    while(!feof(fp1)) {
        fgets(sub_counter, SUB_COUNTER_CHARS, fp1);// sub_counter contains the '\n'.

        /*
         * If the following condition is true, three consecutive '\n'
         * have been found. This could only happen if the subtitle
         * file has ended (in plenty srt files, the end of the file is 
         * denoted by consecutive carriage returns).
         */
        if (strcmp(sub_counter, "\n") == 0) break;
        
        fscanf(fp1, "%s --> %s", init_time, end_time); fgetc(fp1);
        fprintf(fp2, "%s%s --> %s\n", sub_counter, init_time, end_time);

        while(1) {
            if(fgetc(fp1) == '\n') {// First '\n'
                if (fgetc(fp1) == '\n' || feof(fp1)) { // Second '\n' in the row means the end of the subtitle.
                    break;
                }
            }
        }
    }
    fclose(fp1);
    fclose(fp2);

    return 0;
}