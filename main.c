/*
 Main function of subtitles delay project.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/

#include <stdlib.h>
#include <string.h>

#include "sub_delay.h"

int main(void){
    char *subtitle_fname;
    char tmp_fname[] = "tmp_fname.srt";
    int delay_ms = -1;
    
    /* The first srt file found inside the current directory. */
    subtitle_fname = find_srt_file();

    if(subtitle_fname != NULL){
        printf("Subtitle file found --> %s\n", subtitle_fname);
    }
    else {
        fprintf(stderr, "*** No .srt file found! ***\n");
        return 1;
    }

    printf("- Enter delay (+/-) in milliseconds.\n");
    printf("- Delay: ");
    
    while (1) {
        scanf("%d", &delay_ms);
        getchar();
        if (delay_ms >= MIN_DELAY_MS && delay_ms <= MAX_DELAY_MS) {
            break;
        }
        else if (delay_ms == -1) {
            printf("Closing...\n");
            return 0;
        }
        fprintf(stderr, "Invalid delay given. Please try again.\n");
    }

    add_delay_to_file(subtitle_fname, tmp_fname, delay_ms);
    rename_subtitle_file(subtitle_fname, tmp_fname);

    printf("------------------------------------------------\n");
    printf("%d ms delay added to %s!\n", delay_ms, subtitle_fname);
    printf("Original file renamed to --> \"old_%s\"\n", subtitle_fname);
    printf("------------------------------------------------\n");
    printf("Press [ENTER] to exit... ");
    getchar();

    free(subtitle_fname);

    return 0;
}