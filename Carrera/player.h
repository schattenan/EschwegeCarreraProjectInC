#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "stdbool.h"
#include <string.h>

typedef struct {  

	int rank;  
	int track; 
	int device; 
	int rounds;  
	int bestRound; 
	char playername[20];
	bool banned;
	bool finished;
	clock_t roundTime[99];
	clock_t startTime;
	clock_t endTime;

}PLAYER;

void initPLAYER ( PLAYER *start);
void nextRound(PLAYER *start, clock_t currentTime);


