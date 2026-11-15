/*
 * global.c
 *
 * Shared state of the traffic light application
 */

#include "global.h"

app_mode_t mode = MODE_NORMAL;
phase_t phase = R_G;

int time_red = DEFAULT_RED;
int time_amber = DEFAULT_AMBER;
int time_green = DEFAULT_GREEN;

int countdown_road1 = DEFAULT_RED;
int countdown_road2 = DEFAULT_GREEN;
