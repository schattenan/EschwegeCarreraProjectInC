#include "modes.h"

void exportCSV(RACE *ret);

/*

The function exports all race data into a csv-File.
Afterwards it is opening the webpage for viewing and printing the results

*/

void exportTime(FILE *datei, RACE *ret, int player, int round);

/*

The function prints the roundtime of a specific player's round into a file

*/

void exportTotalTime(FILE *datei, RACE *ret, int player);

/*

The function prints the total time of a specific player into a file

*/