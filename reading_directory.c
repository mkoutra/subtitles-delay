// Reading a directory
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main(void){
    DIR *folder;
    struct dirent *entry;
    int nfiles = 0;

    folder = opendir(".");
    if (folder == NULL){
        fprintf(stderr, "Problem opening the directory.");
        return 1;
    }
    // Print the files inside a folder.
    while(entry = readdir(folder)){
        nfiles++;
        printf("#%d.\t%s\n", nfiles, entry->d_name);
    }

    closedir(folder);

    return 0;
}