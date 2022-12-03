// Find the .srt file in the current working directory
// The following function works both on LINUX and on Windows with MinGW gcc compiler.
// We assume that the directory contains only ONE .srt file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/*
 * Returns the name of the .srt file in the current directory
 * Assumes directory contains maximum ONE .srt file.
 * If no file is found returns NULL.
 * After usage, the string it returns needs to be deallocated.
 */   
char *find_srt_file(void){
    char *search_word = ".srt";
    char *file_found;
    DIR *folder;
    struct dirent *entry;

    folder = opendir(".");
    /*
    * Another way to find current working directory instead of ".":
    * char cwd[261];
    * getcwd(cwd, sizeof(cwd));
    * folder = opendir(cwd);
    */

    while(entry = readdir(folder)){
        if (strstr(entry->d_name, search_word)!=NULL){
            file_found = (char *)malloc(strlen(entry->d_name)+1);
            strcpy(file_found, entry->d_name);
            return file_found;
        }
    }

    return NULL;
}