/*
 Functions used in subtitles delay project.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#include "sub_delay.h"

/*****************************************************************************/
/**************************** Find subtitles *********************************/
/*****************************************************************************/

/* Checks if the given filename has the specified extension. */
bool check_extension(const char* fname, const char* ext) {
    int fname_size = strlen(fname);
    int ext_size = strlen(ext);

    if (strcmp(fname + fname_size - ext_size, ext) == 0) {
        return true;
    }
    return false;
}

/*
 * Returns a dynamically allocated Sub_files struct
 * containing all the .srt filenames found in the current directory.
*/
Sub_files* find_all_subs(void) {
    DIR *folder;
    struct dirent* entry;
    char* sub_extensions[N_SUB_EXTENSIONS] = {".srt", ".sub"};
    char *fname = NULL;
    int f_counter = 0;
    Sub_files* subtitle_files;
    
    subtitle_files = (Sub_files*) malloc(sizeof(Sub_files));
    subtitle_files->n = 0;

    folder = opendir(".");
    if (folder == NULL) {
        fprintf(stderr, "Error in opening current directory.\n");
        return NULL;
    }

    /* Find the number of files containing a subtitle extension */
    while( (entry = readdir(folder)) ){
        /* Check for all extensions */
        for (int i = 0; i < N_SUB_EXTENSIONS; ++i) {
            if (check_extension(entry->d_name, sub_extensions[i])) {
                subtitle_files->n++;
            }
        }
    }

    closedir(folder);

    /* No subtitle file found */
    if (subtitle_files->n == 0) {
        // fprintf(stderr, "No subtitle file found.\n");
        return NULL;
    }

    /* Allocate the array to store the subtitle filenames found */
    subtitle_files->filenames = (char**)malloc(subtitle_files->n * sizeof(char*));

    folder = opendir(".");
    if (folder == NULL) {
        fprintf(stderr, "Error in opening current directory.\n");
        return NULL;
    }

    /* Copy filename to Sub_files's filename array */
    while( (entry = readdir(folder)) ){
        /* Check for all extensions */
        for (int i = 0; i < N_SUB_EXTENSIONS; ++i) {
            if (check_extension(entry->d_name, sub_extensions[i])) {
                fname = (char*) malloc(strlen(entry->d_name) + 1);
                strcpy(fname, entry->d_name);
                subtitle_files->filenames[f_counter++] = fname;  
            }
        }
    }
    closedir(folder);

    return subtitle_files;
}

/* Deallocate the Sub_files struct given. */
void destroy_Sub_files(Sub_files* sfiles) {
    /* Deallocate the strings of filenames array. */
    for (int i = 0; i < sfiles->n; ++i) {
        free(sfiles->filenames[i]);
    }

    /* Deallocate the array */
    free(sfiles->filenames);

    free(sfiles);
}

/*****************************************************************************/
/*************************** Modify subfiles *********************************/
/*****************************************************************************/

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
        // fprintf(stderr, "No new segment found\n");
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
    printf("- %d ms delay added to \"%s\"\n", delay, input_fname);

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
    printf("- Original file renamed to --> \"old_%s\"\n", init_fname);

    return 0;
}

/*****************************************************************************/
/***************************** Get user input ********************************/
/*****************************************************************************/

/*
 * Checks if the string given represents an integer
 * Note: It does NOT ignore whitespaces.
*/
bool is_number(const char *str) {
    if (str == NULL || *str == '\0') return false;

    char *endptr;
    strtol(str, &endptr, 10);

    /*
    * If endptr points to the null terminator, then the
    * entire input was parsed successfully as a number.
    */
    return *endptr == '\0';
}

/*
 * Asks a user to choose among the subtitle files and returns its choice.
 * The number of available subtitle files must be passed in n_files. 
*/
int get_user_file_choice(int n_files) {
    char input[MAX_USER_INPUT_SIZE];
    int n_choice = -1;
    while (1) {
        printf("\n- Select a file by typing its number: ");
        fgets(input, MAX_USER_INPUT_SIZE, stdin);

        /* Remove trailing whitespace */
        input[strcspn(input, "\r\n")] = '\0';

        if (!is_number(input)) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        n_choice = atoi(input);

        if (n_choice < 1 || n_choice > n_files) {
            printf("Invalid number choice. Please try again\n");
            continue;
        }

        return n_choice;
    }
}

/*
 * Ask user to add delay in milliseconds to
 * the file specified and returns its answer.
*/
int get_user_delay(void) {
    int delay_ms;
    char input[MAX_USER_INPUT_SIZE];

    while (1) {
        printf("- Enter delay (+/-) in milliseconds: ");
        fgets(input, MAX_USER_INPUT_SIZE, stdin);
        
        /* Remove trailing whitespace */
        input[strcspn(input, "\r\n")] = '\0';

        if (!is_number(input)) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        delay_ms = atoi(input);

        if (delay_ms < MIN_DELAY_MS || delay_ms > MAX_DELAY_MS) {
            printf("Invalid delay. Please try again: ");
            continue;
        }

        return delay_ms;
    }
}

/* Print the filenames of subtitle files located in the current directory. */
void print_subfiles(Sub_files* sfiles) {
    printf("- Subtitles found in current directory: %d\n", sfiles->n);

    for (int i = 0; i < sfiles->n; ++i) {
        printf("  (%d) %s\n", i + 1, sfiles->filenames[i]);
    }
}