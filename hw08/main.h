#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

#define SCORE_ROW 10
#define SCORE_COL 175

void delay(int n);

struct fishes {
    int row;
    int col;
    int width;
    int height;
    int uvel;
    int hvel;
};

struct predator {
    int row;
    int col;
    int width;
    int height;
    int type;
};

struct coralreef {
    int row;
    int col;
    int width;
    int height;
};

#endif
