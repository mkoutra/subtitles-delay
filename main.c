/*
 Main function of subtitles delay tool.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/

#include <stdlib.h>
#include <string.h>

#include "sub_delay.h"

int main(void){
    Sub_files *all_subfiles = NULL;
    int n_choice = 0;
    char* fname_choice = NULL;
    char tmp_fname[] = "tmp_fname.srt";
    int delay_ms = -1;
    
    /* The srt files found inside the current directory. */
    all_subfiles = find_all_subs();

    if (all_subfiles == NULL) {
        printf("*** No .srt file found! ***\n");
        printf("Press [ENTER] to exit... ");
        getchar();
        return 1;
    }

    printf("- Subtitles found in current directory: %d\n", all_subfiles->n);

    for (int i = 0; i < all_subfiles->n; ++i) {
        printf("  (%d) %s\n", i + 1, all_subfiles->filenames[i]);
    }
    while (1) {
        printf("\n- Select a file by typing its number: ");
        scanf("%d", &n_choice); getchar();

        if (n_choice < 1 || n_choice > all_subfiles->n) {
            printf("Invalid number choice. Please try again\n");
        }
        else {
            fname_choice = all_subfiles->filenames[n_choice - 1];
            break;
        }
    }

    printf("- Enter delay (+/-) in milliseconds: ");
    
    while (1) {
        scanf("%d", &delay_ms);
        getchar();
        if (delay_ms >= MIN_DELAY_MS && delay_ms <= MAX_DELAY_MS) {
            break;
        }
        else {
            printf("Invalid delay. Please try again: ");
        }
    }

    add_delay_to_file(fname_choice, tmp_fname, delay_ms);
    rename_subtitle_file(fname_choice, tmp_fname);

    printf("\n- %d ms delay added to \"%s\"\n", delay_ms, fname_choice);
    printf("- Original file renamed to --> \"old_%s\"\n", fname_choice);
    printf("\nPress [ENTER] to exit... ");
    getchar();

    /* Deallocate memory */
    destroy_Sub_files(all_subfiles);

    return 0;
}