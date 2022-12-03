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
