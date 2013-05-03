#include "player.h"

void initPLAYER ( PLAYER *start) {  //initiation of the player struct, necessary in any means

	int i;
    start->rank=0;
    start->track=0;
    start->device=0;
    start->rounds=-1;
    start->bestRound=1;
    start->banned=false;
	start->finished=false;
	start->started=false;
	for(i=0;i<99;i++)
	{
		start->roundTime[i]=0;
	}
}

void nextRound(PLAYER *start) {

	// --------- Setting dedicated lap times ---------
	int i=0;

	//This is due how it works, at the beginning, before the game starts, every player has a round count of -1
	if(start->rounds>-1)  //if player is crossing the line
	{
		if(start->started)
		{
			//Initiate new round
			start->rounds++;
			start->roundTime[start->rounds]=clock();
			start->roundTime[start->rounds+1]=clock();  //Important for export

			
			// --------- Checking for best laps---------   // @TODO: right now this solution does not work
			if(start->rounds>0)
				if( (start->roundTime[start->bestRound]- start->roundTime[start->bestRound-1]) > (start->roundTime[start->rounds]- start->roundTime[start->rounds-1]) && !start->finished)
				{
						start->bestRound=start->rounds;
						printf("%c",7);
				}
				else {}
			else
				start->bestRound=1;
		}

		else
		{
			start->started=true;
		}

	}
	else 
	{
		//Start the game, initiation
			start->rounds++;
			start->startTime=clock();
			start->roundTime[start->rounds+1]=clock();
	}
}  
