#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(void){
    DIR *folder;
    struct dirent *entry;
    char *fname = "aaa.txt";

    folder = opendir(".");
    while (entry = readdir(folder)){
        if (strcmp(entry->d_name, fname) == 0) rename(fname, "bbb.txt");
    }


    return 0;
}