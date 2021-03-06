#include "export.h"

/*

====== stats.csv ======

Mode
Number of Players
Number of Rounds
Playernames
Roundtimes
Ranking
Best round
Total time
End signal


*/

void exportCSV(RACE *ret)
{
	FILE *datei;
	int i,j,roundEnd;

	if( (datei = fopen(EXPORTPATH,"w")) == NULL )
	{
		printf("\n\n\n\n\nFailure accessing %s \n",EXPORTPATH);
		system("pause");
		exit(1);
	}


	//  ==  Game Mode ==
	if(ret->match_Active)
		fprintf(datei,"Rennen");
	if(ret->knockOut_Active)
		fprintf(datei,"Knock Out");
	if(ret->timeAttack_Active)
		fprintf(datei,"Zeitrennen");
	fprintf(datei,"\n");

	//  ==  Number of Players ==
	fprintf(datei,"%d",ret->numberOfPlayers);
	fprintf(datei,"\n");

	//  ==  Number of the max. Rounds ==
	if(ret->timeAttack_Active) fprintf(datei,"%d",ret->maxRounds-1);
	if(!ret->timeAttack_Active)fprintf(datei,"%d",ret->maxRounds);
	fprintf(datei,"\n");


	//  ==  Player names ==
	for(i=0;i<ret->numberOfPlayers;i++)
	{
		fprintf(datei,"%s",ret->players[i].playername);
		if(i<ret->numberOfPlayers-1)
			fprintf(datei,"%c",SEPERATOR);
	}
	fprintf(datei,"\n");

	
	if(ret->timeAttack_Active){
		roundEnd=ret->maxRounds;
	}else {
		roundEnd=ret->maxRounds+1;

	}

	//  ==  Round time for each player for each round ==
	for(i=1;i<roundEnd;i++)
	{
		for(j=0;j<ret->numberOfPlayers;j++)
		{
			exportTime(datei,ret,j,i);
			if(j<ret->numberOfPlayers-1)
				fprintf(datei,"%c",SEPERATOR);
		}
		fprintf(datei,"\n");
	}

	//  ==  Ranking ==
	for(i=0;i<ret->numberOfPlayers;i++)
	{
		fprintf(datei,"%d",ret->players[i].rank);
		if(i<ret->numberOfPlayers-1)
			fprintf(datei,"%c",SEPERATOR);
	}
	fprintf(datei,"\n");

	//  ==  Best Round (TBD)  ==
	for(i=0;i<ret->numberOfPlayers;i++)
	{
		if(ret->timeAttack_Active) fprintf(datei,"%d",ret->players[i].bestRound);
		else fprintf(datei,"%d",ret->players[i].bestRound);
		if(i<ret->numberOfPlayers-1)
			fprintf(datei,"%c",SEPERATOR);
	}
	fprintf(datei,"\n");

	//  ==  Total time for each player ==
	for(i=0;i<ret->numberOfPlayers;i++)
	{
		exportTotalTime(datei,ret,i);
		if(i<ret->numberOfPlayers-1)
			fprintf(datei,"%c",SEPERATOR);
	}
	fprintf(datei,"\n");

	//End sequence
	fprintf(datei,"End");

	fclose(datei);
}

void exportTime(FILE *datei, RACE *ret, int player, int round)  
{
	int minuten,sekunden,millesekunden;

	if( (round > ret->players[player].rounds)  )
		fprintf(datei," ");
	else{
		if(round > (ret->players[player].rounds-1) && ret->timeAttack_Active)
		{
			fprintf(datei," ");
		}
		else
		{
			//Getting the time, very similar to function printTime in modes.c
			if(round==0)
				millesekunden = ret->players[player].roundTime[0]-ret->startTime;
			else
				millesekunden = ret->players[player].roundTime[round]-ret->players[player].roundTime[round-1];

			//Some crazy math nobody understands so don't even try, you will fail anyway ;-)
			sekunden = millesekunden / 1000;
			minuten = sekunden / 60;
			millesekunden = millesekunden - 1000*sekunden;	
			sekunden = sekunden - 60*minuten;

			fprintf(datei,"%02d:%02d.%03d",minuten,sekunden,millesekunden);
		}
	}
}

void exportTotalTime(FILE *datei, RACE *ret, int player)
{
	int minuten,sekunden,millesekunden;

	//Getting the total time, very similar to function exportTime
	millesekunden = ret->players[player].endTime-ret->startTime;

	//Some crazy math nobody understands so don't even try, you will fail anyway ;-)
	sekunden = millesekunden / 1000;
	minuten = sekunden / 60;
	millesekunden = millesekunden - 1000*sekunden;	
	sekunden = sekunden - 60*minuten;

	fprintf(datei,"%02d:%02d.%03d",minuten,sekunden,millesekunden);
}