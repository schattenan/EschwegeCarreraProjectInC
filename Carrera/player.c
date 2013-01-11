#include "player.h"



void initPLAYER ( PLAYER *start) {  //initiation of the player, necessary in any means, get used to it

    start->rank=0;
    start->track=0;
    start->device=0;
    start->rounds=-1;
    start->bestRound=0;
    start->banned=false;
	start->finished=false;
}

void nextRound(PLAYER *start, clock_t currentTime) {

	// --------- Setting dedicated lap times ---------
	int i=0;
	clock_t timer1;

	if(start->rounds>-1)
	{
		timer1=clock();

		if (! (start->rounds>0)  )
		{
			start->roundTime[start->rounds]= timer1-currentTime;
			//printf(" ---------------------------%u-------------------------",start->roundTime[start->rounds]/1000);
		}
		else
		{
			start->roundTime[start->rounds]=timer1-currentTime/*start->roundTime[start->rounds-1]*/;
			//printf(" ---------------------------%u-------------------------",start->roundTime[start->rounds]/1000);
		}

		// --------- Checking best laps---------
		if(start->bestRound==0)
			start->bestRound=0;
		if( start->roundTime[start->bestRound] > start->roundTime[start->rounds] )
			start->bestRound=start->rounds;

		start->rounds++;
		start->roundTime[start->rounds]=clock();

	}
	else 
	{
			start->rounds++;
			start->startTime=clock();
	}
}  
