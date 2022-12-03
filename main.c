#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "sub_delay.h"

int main(void){
    char *file_name;
    int delay;
    
    // Name of the subtitle file we need to change 
    file_name = find_srt_file();

    if(file_name != NULL){
        printf("File found: %s\n", file_name);
    }
    else {
        fprintf(stderr, "No .srt file found!\n");
        return 0;
    }

    printf("Give delay (positive or negative) in milliseconds.\n");
    printf("Delay: ");
    scanf("%d", &delay); getchar();

    add_delay_to_file(file_name, delay);

    printf("------------------------------------------------\n");
    printf("%s has changed!\n", file_name);
    printf("\"old_%s\" is the original file.\n", file_name);
    printf("------------------------------------------------\n");
    printf("Press [ENTER] to exit... ");
    getchar();
    return 0;
}

/*
 * Returns the name of the .srt file in the current directory
 * Assumes directory contains maximum ONE .srt file.
 * If no file is found returns NULL.
 * After usage, the string it returns needs to be deallocated.
 */  
char *find_srt_file(void){
    char *search_word = ".srt";
    char *file_found;
    DIR *folder;
    struct dirent *entry;

    folder = opendir(".");

    while(entry = readdir(folder)){
        if (strstr(entry->d_name, search_word)!=NULL){
            file_found = (char *)malloc(strlen(entry->d_name)+1);
            strcpy(file_found, entry->d_name);
            
            return file_found;
        }
    }

    return NULL;
}

/*
 * Reads time with format: hh:mm:ss,xxx
 * and transforms it to milliseconds.
 */ 
int time_to_int(const char *time_str){
    int hh, mm, ss, xxx, total;

    sscanf(time_str, "%d:%d:%d,%d", &hh, &mm, &ss, &xxx);

    return xxx + 1000 * (ss + mm*60 + hh*3600);
}

/* 
 * Time given in milliseconds is printed 
 * as: hh:mm:ss,xxx in the stream given.
 */
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

/*
 * Add delay to the subtitles of a .srt file.
 * fname is the name of the subtitle file we need 
 * to add the delay to, delay is given in ms.
 * Renames the initial .srt file as: old_fname
 */
int add_delay_to_file(const char *fname, int delay){
    char *temp_sub_file_name = "temp_name.srt";
    char *old_sub_file_name = (char *)malloc(strlen(fname) + 5);
    char init_time[TIME_CHARS], end_time[TIME_CHARS], sub_counter[SUB_COUNTER_CHARS];
    char ch, ch_after_new_line;
    int init_time_ms, end_time_ms;
    FILE *fp1, *fp2;

    fp1 = fopen(fname, "r");
    if(fp1 == NULL) return -1;
    fp2 = fopen(temp_sub_file_name, "w");
    if (fp2 == NULL) return -1;

    while(!feof(fp1)) {
        /*
         * Although sub_counter contains integers, we prefer to 
         * handle it as a string to be able to check for the
         * end of file.
        */
        fgets(sub_counter, SUB_COUNTER_CHARS, fp1);
        /*
         * If the following condition is true, three consecutive '\n'
         * would've been found. This could only happen if the .srt
         * file has ended (in plenty .srt files, the end of file is 
         * denoted by consecutive carriage returns).
         */
        if (strcmp(sub_counter, "\n") == 0 || strcmp(sub_counter, "") == 0 || feof(fp1)) break;
        
        //read timestamps
        fscanf(fp1, "%s --> %s", init_time, end_time); fgetc(fp1);
        
        //transform timestamps to integers (milliseconds)
        init_time_ms = time_to_int(init_time);
        end_time_ms = time_to_int(end_time);
        
        // Write subtitle counter on the new file
        fprintf(fp2, "%s", sub_counter);// sub_counter contains '\n'.
        
        // Write timestamps on the new file.
        time_to_text(init_time_ms + delay, fp2);
        fprintf(fp2, " --> ");
        time_to_text(end_time_ms + delay, fp2);
        fputc('\n', fp2);
        
        // Write text on the new file.
        while(1) {
            ch = fgetc(fp1);
            if(ch == '\n') {// First '\n'
                fputc(ch, fp2);
                ch_after_new_line = fgetc(fp1);
                
                // Second '\n' in the row means the end of the subtitle.
                if (ch_after_new_line == '\n' || feof(fp1)) {
                    fputc(ch_after_new_line, fp2);
                    break;
                }
                else {// Only one '\n' means there is more text on that subtitle
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

    // Renaming
    
    // name of the subtitle file whose timestamps changed.
    sprintf(old_sub_file_name, "old_%s", fname);

    // Initial file is renamed to --> old_previous_filename.
    rename(fname, old_sub_file_name);
    
    // The file containing the new timestamps renamed as the initial file.
    rename(temp_sub_file_name, fname);

    free(old_sub_file_name);

    return 0;
}