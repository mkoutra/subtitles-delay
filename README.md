# Subtitle Delay
**Subtitle Delay** is a simple tool designed to add a time delay to subtitle files. It supports commonly used subtitle formats such as `.srt` and `.sub`, allowing users to easily adjust the timing of subtitles for synchronization with audio or video content.

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
1. Place the executable in the directory with your subtitle files.
2. Run the executable. If multiple subtitle files are present in the execution directory, the program will prompt you to select the file for modification; otherwise, it will automatically modify the sole subtitle file in the current directory.
3. Enter a time delay in **milliseconds** (1 millisecond = 0.001 seconds).
   - Positive delays move subtitles forward; negative ones move them backward.
   - The maximum delay is &plusmn; 5 minutes (&plusmn;300000 ms).
4. The selected file will be modified, while the original version will remain available with the prefix "old_" added to its name.

## TODO
 - Implement an alternative usage with command-line arguments (argc and argv).