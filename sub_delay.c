#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "sub_delay.h"

/*
 * Returns the name of the .srt file in the current directory.
 * Assumes the directory contains a maximum of ONE .srt file.
 * If no file is found, returns NULL.
 * After usage, the string returned needs to be deallocated.
*/
char *find_srt_file(void) {
    char *search_word = ".srt";
    char *file_found;
    DIR *folder;
    struct dirent *entry;

    folder = opendir(".");

    while( (entry = readdir(folder)) ){
        if (strstr(entry->d_name, search_word) != NULL){
            file_found = (char*) malloc(strlen(entry->d_name) + 1);
            strcpy(file_found, entry->d_name);
            
            return file_found;
        }
    }

    return NULL;
}

/*
 * Transforms time format "hh:mm:ss,xxx" to milliseconds.
 * Returns the time in milliseconds on success, otherwise -1.
*/
int time_to_int(const char *time_str){
    int hh, mm, ss, xxx;

    if (sscanf(time_str, "%d:%d:%d,%d", &hh, &mm, &ss, &xxx) != 4) {
        fprintf(stderr, "Error in time_to_int(): Invalid time format.\n");        
        fprintf(stderr, "\"%s\" is invalid.\n", time_str);
        return -1;
    }

    // printf("hh = %d, mm = %d, ss = %d, xxx = %d\n", hh, mm, ss, xxx);

    return xxx + 1000 * (ss + mm * 60 + hh * 3600);
}

/*
 * Converts a time duration in milliseconds to a timestamp format
 * and stores the result in the provided time_str buffer.
 * Returns 0 if successful, otherwise -1.
 * */
int time_to_str(int time_ms, char* time_str) {
    int hh, mm, ss, xxx;

    hh = time_ms / (3600 * 1000);
    time_ms %= (3600 * 1000);
    mm = time_ms / (60 * 1000);
    time_ms %= (60 * 1000);
    ss = time_ms / 1000;
    xxx = time_ms % 1000;

    if (sprintf(time_str, "%02d:%02d:%02d,%03d", hh, mm, ss, xxx) < 0) {
        fprintf(stderr, "Failure on time_to_str().\n");
        return -1;
    }

    return 0;
}

/* 
 * Adds the specified delay (in milliseconds) to the provided
 * timestamps.The delay can be both positive or negative.
 * Returns 0 if successful; otherwise, returns -1.
*/
int add_delay(int delay, char* init_timestamp, char* final_timestamp) {
    int init_time_ms;
    int final_time_ms;

    /* Transform string timestamps to milliseconds. */
    init_time_ms = time_to_int(init_timestamp);
    if (init_time_ms == -1) {
        fprintf(stderr, "Error in add_delay\n");
        return -1;
    }

    final_time_ms = time_to_int(final_timestamp);
    if (final_time_ms == -1) {
        fprintf(stderr, "Error in add_delay\n");
        return -1;
    }
    
    /* Add delay to timestamp strings */
    if (time_to_str(init_time_ms + delay, init_timestamp) == -1) {
        fprintf(stderr, "Error in adding delay to \
                init_timestamp in add_delay().\n");
        return -1;
    }

    if (time_to_str(final_time_ms + delay, final_timestamp) == -1) {
        fprintf(stderr, "Error in adding delay to \
                final_timestamp in add_delay().\n");
        return -1;
    }

    return 0;
}

/* 
 * Reads a subtitle segment from the file input_fp,
 * adds the specified delay in milliseconds to its timestamps,
 * and writes the segment with new timestamps to the output file.
 * If no segment is found, the function returns -1;
 * otherwise, it returns 0 (indicating success).
*/
int add_delay_to_segment(FILE* input_fp, FILE* output_fp, int delay_ms) {
    /* Try to read the first line as string to 
     * solve the HUGE issue occured with BOM
     * at the beginning.
    */
    char sub_id[SUB_ID_MAX_LENGTH];         /* Subtitle id */
    char init_time_str[TIMESTAMPS_CHARS];   /* Initial timestamp */
    char final_time_str[TIMESTAMPS_CHARS];  /* Final timestamp */
    char buffer[LINE_MAX_SIZE];             /* A subtitle text line */        
    char time_line[TIME_LINE_MAX_LENGTH];   /* Line containing timestamps */

    /* Read subtitle segment. */
    fgets(sub_id, SUB_ID_MAX_LENGTH, input_fp);

    if (strcmp(sub_id, "\n") == 0 || strcmp(sub_id, "\r\n") == 0) {
        fprintf(stderr, "No new segment found\n");
        return -1;
    }

    fgets(time_line, TIME_LINE_MAX_LENGTH, input_fp);
    sscanf(time_line, "%s --> %s", init_time_str, final_time_str);

    /* Insert delay to timestamps. */
    if (add_delay(delay_ms, init_time_str, final_time_str) == -1) {
        fprintf(stderr, "Error in add_delay_segment()\n");
        return -1;
    }

    /* Write id and timestamps to output file */
    fprintf(output_fp, "%s", sub_id);
    fprintf(output_fp, "%s --> %s\n", init_time_str, final_time_str);

    /*
     * Read subtitle text from input file and write to output file.
     * An Empty line means end of segment.
     */
    do {
        fgets(buffer, LINE_MAX_SIZE, input_fp);
        fprintf(output_fp, "%s", buffer);
    } while(strcmp(buffer, "\n") != 0 && strcmp(buffer, "\r\n") != 0);

    return 0;
}

/* 
 * Reads a subtitle file with the name input_fname,
 * adds the specified delay (in milliseconds) to its timestamps,
 * and generates a subtitle file with the name output_fname
 * containing the initial subtitles with modified timestamps.
 * Returns 0 if successful, -1 otherwise.
*/
int add_delay_to_file(const char* input_fname,
                      const char* output_fname, int delay) {
    FILE *input_fp, *output_fp;
    
    /* Initial subtitle file. */
    input_fp = fopen(input_fname, "r");
    if (input_fp == NULL) {
        fprintf(stderr, "Problem opening input file.\n");
        return -1;
    }

    /* File containing the modified subtitle timestamps. */
    output_fp = fopen(output_fname, "w");
    if (output_fp == NULL) {
        fprintf(stderr, "Problem opening output file.\n");
        return -1;
    }

    while(!feof(input_fp)) {
        if (add_delay_to_segment(input_fp, output_fp, delay) == -1) {
            break;
        }
    }

    /* Needed to signify end of .srt file*/
    fprintf(output_fp, "\n\n");

    fclose(input_fp);
    fclose(output_fp);

    return 0;
}

/*
 * Renames two files:
 * (1) Renames init_fname --> old_init_fname
 * (2) Renames tmp_fname --> init_fname
 * After implementation, the file with the initial filename contains
 * the modified timestamps, while the initial filename contains the
 * prefix 'old_'.
 * Returns 0 if successful, otherwise -1.
*/
int rename_subtitle_file(char* init_fname, char* tmp_fname) {
    char *old_prefix_fname = (char *) malloc(strlen(init_fname) \
                                              + OLD_PREFIX_LENGTH);
    
    sprintf(old_prefix_fname, "old_%s", init_fname);

    /* Add 'old_' to initial filename */
    if (rename(init_fname, old_prefix_fname) != 0) {
        fprintf(stderr, "Error renaming initial file.\n");
        return -1;
    }
    
    /* Rename the file containing the modified timestamps */
    if (rename(tmp_fname, init_fname) != 0) {
        fprintf(stderr, "Error renaming temporary file.\n");
        return -1;
    }

    free(old_prefix_fname);

    return 0;
}