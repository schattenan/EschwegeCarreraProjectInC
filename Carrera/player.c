#include "player.h"

void initPLAYER ( PLAYER *start) {  //initiation of the player struct, necessary in any means

	int i;
    start->rank=0;
    start->track=0;
    start->device=0;
    start->rounds=-1;
    start->bestRound=0;
    start->banned=false;
	start->finished=false;
	for(i=0;i<99;i++)
	{
		start->roundTime[i]=0;
	}
}

void nextRound(PLAYER *start, clock_t currentTime) {

	// --------- Setting dedicated lap times ---------
	int i=0;
	clock_t timer1;

	//This is due how it works, at the beginning, before the game starts, every player has a round count of -1
	if(start->rounds>-1)  //if player is crossing the line
	{
		timer1=clock();

		start->roundTime[start->rounds] = timer1-currentTime;  //Set the total time of the finished round

		// --------- Checking for best laps---------
		if(start->bestRound==0)
			start->bestRound=0;
		if( start->roundTime[start->bestRound] > start->roundTime[start->rounds] )
			start->bestRound=start->rounds;

		//Initiate new round
		start->rounds++;
		start->roundTime[start->rounds]=clock();
		start->roundTime[start->rounds+1]=clock();  //Important for export

	}
	else 
	{
		//Start the game, initiation
			start->rounds++;
			start->startTime=clock();
			start->roundTime[start->rounds+1]=clock();
	}
}  
