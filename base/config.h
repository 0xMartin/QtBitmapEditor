#ifndef CONFIG_H
#define CONFIG_H


// defaultni obnovovaci vzdalenost pro pohyb kurzoru pri meritku (100% - 1.0)
#define DEFAULT_MOUSE_HELPER_DIST 5

// minimalni scale projektu nutne pro zobrazeni pixelove mrizky
#define PIXEL_GRID_MIN_SCALE 15

// minimalni delka jmena projektu
#define PROJECT_MIN_NAME_LENGTH 3

// koncovka projektoveho souboru
#define PROJECT_FILE_EXTENSION ".qbe"

// minimalni velikost tuzka/stetce/gumy
#define MIN_TOOL_SIZE 1

// maximalni velikost tuzka/stetce/gumy
#define MAX_TOOL_SIZE 1000

// zakladni velikost tuzka/stetce
#define DEFAULT_TOOL_SIZE 25

// zakladni tolerance pro nastroj "fill color"
#define DEFAULT_FILL_TOLERANCE 10

// minimalni pozice
#define POSITION_MIN -99999

// maximalni pozice
#define POSITION_MAX 99999


#endif // CONFIG_H
