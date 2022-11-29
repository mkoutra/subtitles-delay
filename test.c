#include <stdio.h>
#include <string.h>

int main(void){
    char buffer[10], ch;
    int num1, num2;
    FILE *fp;
    fp = fopen("test_file_2.txt", "r");
    
    while(!feof(fp)) {
        fgets(buffer, 10, fp);
        if (strcmp(buffer, "\n") == 0) printf("aaa\n");
        printf("buffer = %s", buffer);
        fscanf(fp, "%c %d", &ch, &num2);
    }
    printf("%c\t%d\n", ch, num2);


    return 0;
}