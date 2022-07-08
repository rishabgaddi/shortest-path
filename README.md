# Advanced C Project -  Compute shortest path between two cities

This project contains the following files-
1. status.h
2. List.h
3. map.h
4. status.c
5. List.c
6. map.c
7. main.c
8. makefile
9. FRANCE.MAP
10. README.md

In order to build and run the program,
1. Open the terminal/powershell/command prompt.
2. Navigate to this project directory
3. Run command - make (It will generate a runnable program in the same directory named 'aStar')
4. Run command - ./aStar <source-city-name> <destination-city-name> (Example: ./aStar Rennes Lyon)

Note:
The main.c file requires FRANCE.MAP to be available in the current directory in order to construct the map.
You can replace the FRANCE.MAP with any other map file, but make sure you provide the details similar to the one in FRANCE.MAP.
Also, make sure you replace the new map file name in the main.c file.
