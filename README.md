# Subtitle Delay
This tool adds a time delay to a subtitle file.

## Compiling
You can create the `subtitle_delay` executable using one of the following methods:

1. Using CMake to generate the Makefile:
   ```console
   cmake .
   make
   ```
2. Using the provided, Makefile:
   ```console
   make
   ```
3. Assuming the compiler is gcc, open a terminal window in the current directory and type:
   ```console
   gcc -g main.c sub_delay.c sub_delay.h -o subtitle_delay.exe
   ```
Note: Executables for Linux x86_64 and Windows 10 are provided in the corresponding directories.

## ATTENTION
Currently, the program assumes there is only **one .srt** subtitle file in the corresponding directory. Ensure there is **only one** file with a **.srt** extension in the current director.

## USAGE
After compiling, place the executable in the directory containing the subtitle file you want to modify. When executed, the program prompts the user to enter a time delay in **milliseconds**. The time interval can be positive or negative, indicating moving the subtitles forward or backward.

## TODO
 - Implement an alternative usage with command-line arguments (argc and argv).
 - Handle the possibility of alternative subtitle file extensions, such as .sub and .txt.
 - Address cases where there is more than one subtitle file in the current directory.
