# Subtitle Delay
This tool adds a time delay to a .srt subtitle file.

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

## Executables
Executable files for x86_64 Linux and Windows are available in their respective directories.

## Usage
- Place the executable in the directory with your subtitle files.
- Run the program, and it will prompt you to select the subtitle file for modification.
- Enter a time delay in **milliseconds** (1 millisecond = 0.001 seconds).
- Positive delays move subtitles forward; negative ones move them backward.
- The maximum delay is 5 minutes (300000 ms).

## TODO
 - Implement an alternative usage with command-line arguments (argc and argv).
 - Handle the possibility of alternative subtitle file extensions, such as .sub and .txt.
