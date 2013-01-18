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
	int i,j;

	if( (datei = fopen(EXPORTPATH,"w")) == NULL )
	{
		printf("Failure accessing stats.csv");
		exit(0);
	}


	//  ==  Game Mode ==
	if(ret->match_Active)
		fprintf(datei,"Rennen");
	if(ret->knockOut_Active)
		fprintf(datei,"Der Letzte fliegt");
	if(ret->timeAttack_Active)
		fprintf(datei,"Time Attack");
	fprintf(datei,"\n");

	//  ==  Number of Players ==
	fprintf(datei,"%d",ret->numberOfPlayers);
	fprintf(datei,"\n");

	//  ==  Number of the max. Rounds ==
	fprintf(datei,"%d",ret->maxRounds);
	fprintf(datei,"\n");


	//  ==  Player names ==
	for(i=0;i<ret->numberOfPlayers;i++)
	{
		fprintf(datei,"%s",ret->players[i].playername);
		if(i<ret->numberOfPlayers-1)
			fprintf(datei,"%c",SEPERATOR);
	}
	fprintf(datei,"\n");

	//  ==  Round time for each player for each round ==
	for(i=0;i<ret->maxRounds;i++)
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
		fprintf(datei,"%d",ret->players[i].bestRound);
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

	system(IMPORTPATH);  //Opens the website for printing

}

void exportTime(FILE *datei, RACE *ret, int player, int round)  
{
	int minuten,sekunden,millesekunden;

	//Getting the time, very similar to function printTime in modes.c
	if(round==0)
		millesekunden = ret->players[player].roundTime[0]-ret->players[player].startTime;
	else
		millesekunden = ret->players[player].roundTime[round]-ret->players[player].roundTime[round-1];

	//Some crazy math nobody understands so don't even try it, you will fail anyway ;-)
	sekunden = millesekunden / 1000;
	minuten = sekunden / 60;
	millesekunden = millesekunden - 1000*sekunden;	
	sekunden = sekunden - 60*minuten;

	fprintf(datei,"%02d:%02d.%03d",minuten,sekunden,millesekunden);
}

void exportTotalTime(FILE *datei, RACE *ret, int player)
{
	int minuten,sekunden,millesekunden;

	//Getting the total time, very similar to function exportTime
	millesekunden = ret->players[player].endTime-ret->players[player].startTime;

	//Some crazy math nobody understands so don't even try it, you will fail anyway ;-)
	sekunden = millesekunden / 1000;
	minuten = sekunden / 60;
	millesekunden = millesekunden - 1000*sekunden;	
	sekunden = sekunden - 60*minuten;

	fprintf(datei,"%02d:%02d.%03d",minuten,sekunden,millesekunden);
}