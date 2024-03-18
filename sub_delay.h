/*
 A header for the subtitles delay project.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/

#ifndef SUB_DELAY_H
#define SUB_DELAY_H

#include <stdio.h>
#include <stdbool.h>

#define TIMESTAMPS_CHARS 13     /* hh:mm:ss,xxx\0 */
#define LINE_MAX_SIZE 100       /* Max length of subtitle text line */
#define OLD_PREFIX_LENGTH 5     /* old_\0 */
#define SUB_ID_MAX_LENGTH 7     /* Subtitle id's string length */
#define TIME_LINE_MAX_LENGTH 40 /* Length of line containing the timestamps */ 
#define MIN_DELAY_MS -300000    /* Minimum valid delay (-5 mins) */
#define MAX_DELAY_MS 300000     /* Maximum valid delay (5 mins) */
#define MAX_USER_INPUT_SIZE 10  /* Max chars to be read when asking for input */
#define N_SUB_EXTENSIONS 2      /* Number of subtitle extensions to search for */

/*****************************************************************************/
/**************************** Find subtitles *********************************/
/*****************************************************************************/

/* Describes the subtitle files found */
typedef struct files_found {
    int n;              /* Number of files found */
    char **filenames;   /* Array containing filenames */
} Sub_files;

/* Checks if the given filename has the specified extension. */
bool check_extension(const char* fname, const char* ext);

/*
 * Returns a dynamically allocated Sub_files struct
 * containing all the .srt filenames found in the current directory.
*/
Sub_files* find_all_subs(void);

/*
 * Deallocate the Sub_files struct given.
 * It is assumed that it was dynamically allocated.
*/
void destroy_Sub_files(Sub_files* sfiles);

/*****************************************************************************/
/*************************** Modify subfiles *********************************/
/*****************************************************************************/

/*
 * Transforms time format "hh:mm:ss,xxx" to milliseconds.
 * Returns the time in milliseconds on success, otherwise -1.
*/
int time_to_int(const char *time_str);

/*
 * Converts a time duration in milliseconds to a timestamp format
 * and stores the result in the provided time_str buffer.
 * Returns 0 if successful, otherwise -1.
 * */
int time_to_str(int time_ms, char* time_str);

/* 
 * Adds the specified delay (in milliseconds) to the provided
 * timestamps.The delay can be both positive or negative.
 * Returns 0 if successful; otherwise, returns -1.
*/
int add_delay(int, char*, char*);

/* 
 * Reads a subtitle segment from the file input_fp,
 * adds the specified delay in milliseconds to its timestamps,
 * and writes the segment with new timestamps to the output file.
 * If no segment is found, the function returns -1;
 * otherwise, it returns 0 (indicating success).
*/
int add_delay_to_segment(FILE* input_fp, FILE* output_fp, int delay_ms);

/* 
 * Reads a subtitle file with the name input_fname,
 * adds the specified delay (in milliseconds) to its timestamps,
 * and generates a subtitle file with the name output_fname
 * containing the initial subtitles with modified timestamps.
 * Returns 0 if successful, -1 otherwise.
*/
int add_delay_to_file(const char* input_fname, const char* output_fname, int delay);

/*
 * Renames two files:
 * (1) Renames init_fname --> old_init_fname
 * (2) Renames tmp_fname --> init_fname
 * After implementation, the file with the initial filename contains
 * the modified timestamps, while the initial filename contains the
 * prefix 'old_'.
 * Returns 0 if successful, otherwise -1.
*/
int rename_subtitle_file(char* init_fname, char* tmp_fname);

/*****************************************************************************/
/***************************** Get user input ********************************/
/*****************************************************************************/

/*
 * Checks if the string given represents an integer
 * Note: It does NOT ignore whitespaces.
*/
bool is_number(const char *str);

/*
 * Asks a user to choose among the subtitle files and returns its choice.
 * The number of available subtitle files must be passed in n_files. 
*/
int get_user_file_choice(int n_files);

/*
 * Ask user to add delay in milliseconds to
 * the file specified and returns its answer.
*/
int get_user_delay(void);

/* Print the filenames of subtitle files located in the current directory. */
void print_subfiles(Sub_files* sfiles);

#endif /* SUB_DELAY_H */