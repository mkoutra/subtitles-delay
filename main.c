/*
 Main function of subtitles delay tool.

 ----------------------------------
 Michail E. Koutrakis
 Github: https://github.com/mkoutra
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
        printf("- No subtitle file found.\n");
        printf("- Please place the executable in a \
                directory containing subtitle files.\n");
        printf("Press [ENTER] to exit... ");
        getchar();
        return 1;
    }

    printf("- Subtitles found in current directory: %d\n", all_subfiles->n);

    for (int i = 0; i < all_subfiles->n; ++i) {
        printf("  (%d) %s\n", i + 1, all_subfiles->filenames[i]);
    }

    /* Single subtitle file inside current directory */
    if (all_subfiles->n == 1) {
        n_choice = 1;
    }
    else {
        /* Ask user to choose among the available files */
        n_choice = get_user_file_choice(all_subfiles->n);
    }

    /* File chosen*/
    fname_choice = all_subfiles->filenames[n_choice - 1];

    /* Ask user for the desired delay */
    delay_ms = get_user_delay();

    /* Add delay to the corresponding file */
    add_delay_to_file(fname_choice, tmp_fname, delay_ms);
    
    /* Rename temp to keep both the initial and the modified subtitle file */
    rename_subtitle_file(fname_choice, tmp_fname);

    printf("\nPress [ENTER] to exit... ");
    getchar();

    /* Deallocate memory */
    destroy_Sub_files(all_subfiles);

    return 0;
}