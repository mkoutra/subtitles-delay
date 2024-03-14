// A header for the subtitles delay project.
#include <stdio.h>// For FILE type. 

#define TIME_CHARS 13 // Time is expressed with 12 characters (hh:mm:ss,xxx).
#define SUB_COUNTER_CHARS 6 // characters to store the subtitle counter. Most srt files contain ~1200 subtitles.

/*
 * Reads time with format: hh:mm:ss,xxx
 * and transforms it to milliseconds.
 */ 
int time_to_int(const char*);

/* 
 * Time given in milliseconds is printed 
 * as: hh:mm:ss,xxx in the stream given.
 */
void time_to_text(int, FILE*);

/*
 * Add delay to the subtitles of a .srt file.
 * fname is the name of the subtitle file we need 
 * to add the delay to, delay is given in ms.
 * Keeps the initial .srt file renamed as: old_fname
 */
int add_delay_to_file(const char *, int);

char *find_srt_file(void);