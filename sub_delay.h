/*
 A header for the subtitles delay project.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/
#ifndef SUB_DELAY_H
#define SUB_DELAY_H

#include <stdio.h>

#define TIMESTAMPS_CHARS 13     /* hh:mm:ss,xxx\0 */
#define LINE_MAX_SIZE 100       /* Max-length of subtitle text line*/
#define OLD_PREFIX_LENGTH 5     /* old_\0 */
#define SUB_ID_MAX_LENGTH 7     /* Subtitle id's string length*/
#define TIME_LINE_MAX_LENGTH 40 /* Length of line containing the timestamps */ 
#define MIN_DELAY_MS 0          /* Minimum valid delay given by user */
#define MAX_DELAY_MS 300000     /* Maximum valid delay given by user (5 min)*/

/*
 * Returns the name of the .srt file in the current directory.
 * Assumes the directory contains a maximum of ONE .srt file.
 * If no file is found, returns NULL.
 * After usage, the string returned needs to be deallocated.
*/
char *find_srt_file(void);

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

#endif /* SUB_DELAY_H */