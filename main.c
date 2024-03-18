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
    
    /* The subtitle files inside the current directory. */
    all_subfiles = find_all_subs();

    if (all_subfiles == NULL) {
        printf("- No subtitle files found in the current directory.\n");
        printf("- Please ensure that the executable is placed in a directory containing subtitle files.\n");
        printf("- Press [ENTER] to exit...\n");
        getchar();
        return 1;
    }

    print_subfiles(all_subfiles);

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
    
    /* Rename temp to keep both the initial and the modified file */
    rename_subtitle_file(fname_choice, tmp_fname);

    printf("\nPress [ENTER] to exit...\n");
    getchar();

    /* Deallocate memory */
    destroy_Sub_files(all_subfiles);

    return 0;
}