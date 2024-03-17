#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Checks if the string given represents an integer
 * Note: It does NOT ignore whitespaces.
*/
bool is_number(const char *str) {
    if (str == NULL || *str == '\0') return false;

    char *endptr;
    strtol(str, &endptr, 10);

    /*
    * If endptr points to the null terminator, then the
    * entire input was parsed successfully as a number.
    */
    return *endptr == '\0';
}

int main(void) {
    char* s = "-1";

    int check = is_number(s);
    printf("check = %d\n", check);

    return 0;
}