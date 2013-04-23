#include "modes.h"
        
int initRACE  (RACE *ret) {
     
	int error;

              ret->finished=false;
              ret->started=false;
			  ret->match_Active=false;
			  ret->knockOut_Active=false;
			  ret->timeAttack_Active=false;
			  ret->activeSensor[0]=false;
			  ret->activeSensor[1]=false;
			  ret->activeSensor[2]=false;
			  ret->activeSensor[3]=false;
			  ret->knockOutPlayerX[0]=0;
			  ret->knockOutPlayerX[1]=0;
			  ret->knockOutPlayerX[2]=0;
			  ret->knockOutPlayerX[3]=0;
			  ret->numberOfPlayers=0;
			  ret->maxTime=999999999;
			  ret->errorcode=0;

			  error = initDEVICE(& ret->device);
              if( error != 0) //Error
			  {
				return error;
			  }

			  ret->playerLine=12;
              initPLAYER(& ret->players[0]);
			  initPLAYER(& ret->players[1]);
			  initPLAYER(& ret->players[2]);
			  initPLAYER(& ret->players[3]);
			  ret->knockOutPlayer=0;

			  strcpy(ret->players[0].playername,"PLAYER_1");
			  strcpy(ret->players[1].playername,"PLAYER_2");
			  strcpy(ret->players[2].playername,"PLAYER_3");
			  strcpy(ret->players[3].playername,"PLAYER_4");

			  return 0;
}

int match (RACE *ret) {

	nextRound(&ret->players[ret->playerLine]);  //sets new round


	if(ret->players[ret->playerLine].rounds == ret->maxRounds)  //player has finished the race
	{
		if (!(setPower( &ret->device,(ret->playerLine),false ) == 0) ) //Power off player 
			return -1;

		ret->knockOutPlayer++;  //one less player active
		ret->players[ret->playerLine].finished=true;
		ret->players[ret->playerLine].endTime=clock();
		ret->players[ret->playerLine].rank = ret->knockOutPlayer;  //ranking, count up 1-4

		 // -------------------  Checks all players  -------------------------- \\ 

		if(ret->knockOutPlayer==ret->numberOfPlayers) //If all player have finished, the race has ended
		{
			ret->finished=true;   //race has ended
		}
	}
	return 0;
}

int knockOut (RACE *ret) {   

	int i;

	if(!ret->players[ret->playerLine].finished)   //If player is finished, he won't be able to drive more rounds
		nextRound(&ret->players[ret->playerLine]);

	if(ret->players[ret->playerLine].rounds>0) {   //All player need at least one round

		ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]++;   //counting, how many players have reached a specific lap number

		//If a player is the last; for example: 4th in the first round, 3rd in the second round, etc..
		if(ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]==ret->numberOfPlayers-ret->players[ret->playerLine].rounds+1)
		{
			ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]=0;   //counter is going to be resseted
			ret->knockOutPlayer++;  //one player out

			ret->players[ret->playerLine].finished=true;
			ret->players[ret->playerLine].endTime=clock();
			for(i=ret->players[ret->playerLine].rounds;i<ret->maxRounds+1;i++) //all future rounds are going to be set (important for export)
				ret->players[ret->playerLine].roundTime[i]= ret->players[ret->playerLine].endTime;

			if( !(setPower( &ret->device,ret->playerLine,false ) == 0) )  //Turn of the player's track
				return -1;

			ret->players[ret->playerLine].rank=ret->numberOfPlayers-ret->players[ret->playerLine].rounds+1; //Set rank, counting down from 4 to 1

			if(ret->knockOutPlayer==ret->numberOfPlayers) //If all players have been knocked out, the game is finished
				ret->finished=true;	
		}

	}
	return 0;
}

void placingTimeAttack(RACE *ret) {

	int i;
	int runde[4]= {ret->players[0].rounds,ret->players[1].rounds,ret->players[2].rounds,ret->players[3].rounds};
	int win[4]={0,0,0,0};
	int platz[4];
	int j,k=0,m;

	//This is a generic sort feature, Part 1 
	for(i=0;i<ret->numberOfPlayers;i++) 
	{
		for(j=ret->players[i].rounds;j<ret->maxRounds+1;j++)  // sets all missing lap times to endTime (important for export)
			ret->players[ret->playerLine].roundTime[j]=clock();
		//n*n
		for(j=0;j<ret->numberOfPlayers;j++)
		{
			if(runde[i]>=runde[j])
				win[i]++;  // counts how many times a player has more rounds finished than another 
		}
	}

	//This is a genric sort feature, Part 2
	for(i=0;i<ret->numberOfPlayers;i++) //Place
	{
		m=k;
		for(j=0;j<ret->numberOfPlayers;j++) //Player
		{
			if(win[j]==ret->numberOfPlayers-i) 
			{
				ret->players[j].rank=m+1;
				platz[j]=m+1;
				k++;
			}			
		}		
	}
}

int run (RACE *ret) {

	HANDLE hThread[2];	
	int i=0;
	int error;

	if(!(countdown(ret)==0))  //countdown
		return -1;

		//Set all tracks active
		for(i=0;i<ret->numberOfPlayers;i++)
		{
			if( !(setPower(&ret->device,i,true)==0) )
			{
				printf("error while setting a port\n Please restart ");
				return -1;
			}				
			ret->players[0].roundTime[i]=clock(); 
		}
		
		hThread[0] = CreateThread(NULL,0, raceloop, ret, 0, NULL);  //Start first thread with function raceloop
		hThread[1] = CreateThread(NULL,0, updateTime, ret, 0, NULL); //Start second thread with function updateTime

		WaitForMultipleObjects( 2, hThread , true , INFINITE  );  //Wait for both threads to be finished

		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);

		if( !(ret->errorcode==0))
			return ret->errorcode;

	printf("\n\n\nRace has been finished");
	updateTime(ret);  //Last time update for ranking
	return 0;
}

void gotoxy(int x, int y)
{

    HANDLE stdOutput;
    COORD pos;

    stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    pos.X = x;
    pos.Y = y; 

    SetConsoleCursorPosition(stdOutput, pos);
}

bool confirm() {

	char a;

	do{
		a=getch();
		if(a==89 || a ==121)  // Y and y , success
		{
			system("cls");
			return true;
		}
		
		if(a==78 || a==110)  // N and n , failure
		{	
			system("cls");
			return false;
		}
	}while(1);  //As long as somebody presses y or n
}

void initUI(RACE *ret) {

	char a;
	int timeInt;
	char rounds[4];
	bool decision=false, test;
	a=0;

	do{
		//system("cls");
		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("\nWhat Game Mode you want to play?\n\n");
		printf("     1) Race\n"); 
		printf("     2) Knock Out\n");
		printf("     3) Time Attack\n\n");

		a=getch();
		test=false;
	
		if(a==49 ||  a==33)
		{
			printf("\n You want to choose Race? (y/n)"); 
			test=confirm();
			if( (test) )
			{
				decision=true;
				ret->match_Active=true;
			}
		}

		if(a==50 ||  a==34)
		{
			printf("\n You want to choose Knock Out? (y/n)"); 
			test=confirm();
			if( (test) )
			{
				decision=true;
				ret->maxRounds=4;  
				ret->knockOut_Active=true;
			}
				
		}
		if(a==51 ||  a==35 ||  a==-11)
		{
			printf("\n You want to choose Time Attack? (y/n)"); 
			test=confirm();
			if( (test) )
			{
				decision=true;
				ret->maxRounds=2;  
				ret->timeAttack_Active=true;
			}
		}

	}while(!decision && !test);  //Cant skip

	if(ret->match_Active)
	{
		do{
			system("cls");
			printf("\nWelcome to the BG Carrera Application \n");
			printf("_____________________________________________\n\n\n");
			printf("\nHow many rounds? (1-99)\n\n");

			test=false;

			fflush(stdin);
			fgets(rounds, 4, stdin);
			ret->maxRounds = atoi(rounds);
			if(ret->maxRounds>0 && ret->maxRounds<100)
			{
				printf("%d Rounds? (y/n)",ret->maxRounds);
				test=confirm();
			}
			else {
				printf("\nYou have choosen %d. Please insert a valid number (1-99)",ret->maxRounds);
				a=getch();
			}
	
		}while(!test);
	}
	if(ret->timeAttack_Active)
	{
		do{
			system("cls");
			printf("\nWelcome to the BG Carrera Application \n");
			printf("_____________________________________________\n\n\n");
			printf("\nHow long? (min) (1-9)\n\n");

			test=false;

			fflush(stdin);
			fgets(rounds, 3, stdin);
			timeInt = atoi(rounds);
			if(timeInt>0 && timeInt<10)
			{
				printf("%d Minutes? (y/n)",timeInt);
				test=confirm();
				ret->maxTime = timeInt*60*CLOCKS_PER_SEC;

			}
			else {
				printf("\nYou have choosen %d. Please insert a valid number (1-9)",ret->maxRounds);
				a=getch();
			}

		}while(!test);
	}


	decision=false;

	do{
		system("cls");
		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("\nHow many players want to participate? (1-4)\n\n");

		a=getch();
		test=false;

		if(a==49 ||  a==33)
		{
			printf("\n You want to choose 1 Player? (y/n)"); 
			ret->numberOfPlayers=1;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==50 ||  a==34)
		{
			printf("\n You want to choose 2 Player? (y/n)"); 
			ret->numberOfPlayers=2;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==51 ||  a==35 ||  a==-11)
		{
			printf("\n You want to choose 3 Player? (y/n)"); 
			ret->numberOfPlayers=3;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==52 ||  a==36)
		{
			printf("\n You want to choose 4 Player? (y/n)"); 
			ret->numberOfPlayers=4;
			test=confirm();
			if( (test) )
				decision=true;
		}

	}while(!decision);

	decision=false;

	do{
		system("cls");
		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("Insert the name of the Player(s)\n\n");

		if(ret->numberOfPlayers>0)printf("     1) %s\n",ret->players[0].playername); 
		if(ret->numberOfPlayers>1)printf("     2) %s\n",ret->players[1].playername);
		if(ret->numberOfPlayers>2)printf("     3) %s\n",ret->players[2].playername);
		if(ret->numberOfPlayers>3)printf("     4) %s\n",ret->players[3].playername); 
		printf("\n     5) Continue\n");


		a=getch();
		test=false;

		if(a==49 ||  a==33)
		{
			printf("\n You want to rename %s? (y/n)",ret->players[0].playername); 
			test=confirm();
			if( (test) )
			{
				getName(ret->players[0].playername);
			}
				
		}
		if((a==50 ||  a==34) && ret->numberOfPlayers>1) 
		{
			printf("\n You want to rename %s? (y/n)",ret->players[1].playername); 
			test=confirm();
			if( (test) )
			{
				getName(ret->players[1].playername);
			}
		}
		if((a==51 ||  a==35 ||  a==-11) && ret->numberOfPlayers>2)
		{
			printf("\n You want to rename %s? (y/n)",ret->players[2].playername); 
			test=confirm();
			if( (test) )
			{
				getName(ret->players[2].playername);
			}
		}
		if((a==52 ||  a==36) && ret->numberOfPlayers>3)
		{
			printf("\n You want to rename %s? (y/n)",ret->players[3].playername); 
			test=confirm();
			if( (test) )
			{
				getName(ret->players[3].playername);
			}
		}
		if(a==53  ||  a==37)
		{
			printf("\n You want to continue? (y/n)"); 
			test=confirm();
			if( (test) )
				decision=true;
		}
	
	}while(!decision);

	decision=false;

	do{
		system("cls");
		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("\nPrepare for the start\n\n");
		printf("    1)We are ready!!!");

		a=getch();
		test=false;

		if(a==49 ||  a==33)
		{
			printf("\n You want to start the race? (y/n)"); 
			test=confirm();
			if( (test) )
				decision=true;
		}
	}while(!decision);

	buildTable(ret);

}

void getName(char name[20]) {

		char a;
		size_t ln;

		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("\nPlease insert the name (at max. 15 characters) \n\n");

		fflush(stdin);
		fgets(name, 15, stdin);  //Max length of name: 15 characters 

		ln = strlen(name) - 1;
		//if (name[ln] == '\n') // if name is complete 
			name[ln] = '\0';  // sign of a finished string
		printf("Your name is '%s' ",name);

		a = getch();
}

void buildTable (RACE *ret) 
{
	int i;

	system("cls");

	gotoxy(0,1);
	printf("        I                I                I                I                 ");

	for(i=0;i<ret->numberOfPlayers;i++)  //Playernames
	{
		gotoxy(10+i*17,1);
		printf("%s",ret->players[i].playername);
	}

	gotoxy(0,2);
	printf("--------I----------------I----------------I----------------I-----------------");

	for(i=0;i<ret->maxRounds;i++) 
	{
		gotoxy(0,i+3);
		printf("Lap %02d  I                I                I                I                \n",i+1);
	}
	gotoxy(0,ret->maxRounds+3);
	printf("--------I----------------I----------------I----------------I-----------------");
	gotoxy(0,ret->maxRounds+4);
	printf("Total   I                I                I                I\n");
	gotoxy(0,ret->maxRounds+5);
	printf("Place   I                I                I                I\n");

}

DWORD WINAPI updateTime(LPVOID data)
{
	int i,j;
	int rounds;
	int lastround[4];

	RACE *ret;
	ret = (int) data;     // pointer is working fine, ignore warning

	rounds = ret->maxRounds;
	lastround[0] = 0;
	lastround[1] = 0;
	lastround[2] = 0;
	lastround[3] = 0;

	do{

		for(i=0;i<ret->numberOfPlayers;i++)  
		{
			if(rounds != ret->maxRounds && ret->timeAttack_Active)
			{
				gotoxy(0,rounds+3);  
				for(j=0;j<150;j++)
					printf(" ");
				gotoxy(0,rounds+3);
				printf("--------I----------------I----------------I----------------I-----------------");
				gotoxy(0,rounds+4);
				printf("Total   I                I                I                I\n");
				gotoxy(0,rounds+5);
				printf("Place   I                I                I                I\n");

				rounds = ret->maxRounds;
			}

			if(ret->players[i].rounds != lastround[i] && ret->players[i].rounds>0)  //If new lap and player has already completet at least one lap
			{
				gotoxy(10+i*17,ret->players[i].rounds+2);  
				printTime(ret,i,ret->players[i].rounds-1,false);  //Print old time one more time to make sure that the right value is printed
				lastround[i]=ret->players[i].rounds;
			}

			if(!ret->players[i].finished && ret->players[i].rounds>=0 && ret->players[i].rounds<ret->maxRounds) //times are updated as long as the player has started and not finsished the game
			{
				gotoxy(10+i*17,ret->players[i].rounds+3);  
				if(!(ret->players[i].rounds==-1))
					printTime(ret,i,ret->players[i].rounds,false);  //Print the time of the players current round

				gotoxy(10+i*17,ret->maxRounds+4);
				printTime(ret,i,ret->players[i].rounds,true);  //Update overall time
			}
			if(ret->players[i].finished)  //If finished
			{
				gotoxy(10+i*17,ret->maxRounds+5);
				printf("    %d.",ret->players[i].rank);   //print rank
			}
		}
	}while( ! (ret->finished) ); //As long as the game is running

	return 0;
}

void printTime(RACE *ret, int player, int round, bool total)
{
	int minuten, sekunden, millesekunden;

		if(total)  // prints the overall time
		{
			millesekunden = (float) ( ret->players[player].roundTime[round+1] - ret->startTime); //current time minus start time
			if(ret->players[player].finished)
				millesekunden = (float) ( ret->players[player].roundTime[ret->players[player].rounds]- ret->startTime ); // round time minus the time of when the race started
		}
		else{	  //prints the current round time		
			millesekunden = (float) ( ret->players[player].roundTime[round+1] - ret->players[player].roundTime[round]);  //current time minus the time of when the lap started
		}
	
	//Some crazy math nobody understands so don't even try it, you will fail anyway ;-)
	sekunden = millesekunden / 1000;
	minuten = sekunden / 60;
	millesekunden = millesekunden - 1000*sekunden;	
	sekunden = sekunden - 60*minuten;
	printf("%02d:%02d.%03d\n",minuten,sekunden,millesekunden);  //print
}

DWORD WINAPI raceloop(LPVOID data) 
{
	int i,j,k;

	RACE *ret;
	ret = (int) data;     // pointer is working fine, ignore warning

	do{
		for(k=0; k < ret->numberOfPlayers ;k++)
		{			
			ret->players[k].roundTime[ret->players[k].rounds+1]=clock();
			if(!(playerCrossesLine(& ret->device, k)==0)) //checks if there are currently player crossing the start point
				goto ERROR1;
				
			if(ret->device.activeTrackSensor[k] && !ret->activeSensor[k] && !ret->players[k].finished) //if player is currently crossing the start
			{     

				ret->playerLine=k;
				ret->activeSensor[k]=true;  //This is neccesarry to make sure that the player just gets one new round at a time

				 //Big issue if there are 2 active game modes

				if(ret->match_Active)
					if(!(match(ret) == 0) )
						goto ERROR2;
				if(ret->knockOut_Active)
					if(!(knockOut( ret) == 0) )
						goto ERROR3;
				if(ret->timeAttack_Active) 
				{
					nextRound(&ret->players[ret->playerLine]);

					//dynamic number of rounds for time attack
					if(ret->players[ret->playerLine].rounds >= ret->maxRounds-1)
					{
						ret->maxRounds++;
					}
					
				}
			}

			if( (!ret->device.activeTrackSensor[k]) && ret->activeSensor[k])  //This is neccesarry to make sure that the player just gets one new round at a time
				ret->activeSensor[k]=false;

			if(  ( ret->players[k].roundTime[ret->players[k].rounds+1] - ret->startTime) >  ret->maxTime && ret->timeAttack_Active) //For TimeAttack, if the maximum time is reached
			{
				//Turn power off for all players
				ret->finished=true;	//race has ended

				for(i=0;i<ret->numberOfPlayers;i++)
				{
					if(!(setPower( &ret->device,i,false ) == 0) ) //Turn off all tracks
						goto ERROR4;

					ret->players[i].finished=true;   
					ret->players[i].endTime=clock();
					for(j=ret->players[i].rounds;j<ret->maxRounds+1;j++)  //Sets all rounds to endTime (important for export)
						ret->players[i].roundTime[j]=ret->players[i].endTime=clock();
				}
				
				placingTimeAttack(ret);  //Afterwards setting the ranks
			}
		}
	}while( !(ret->finished) ); //As long as the game is running

	return 0;

// Errorcodes

ERROR1:  // player crosses line problem
	gotoxy(5,5);
	printf("\n error while getting port status\n Please restart ");
	ret->finished=true;
	ret->errorcode=-1;
	return -1;
ERROR2:  // match problem
	gotoxy(5,5);
	printf("error while setting a port\n Please restart ");
	ret->finished=true;
	ret->errorcode=-2;
	return -2;
ERROR3:  // knock out problem
	gotoxy(5,5);
	printf("error while setting a port\n Please restart ");
	ret->finished=true;
	ret->errorcode=-3;
	return -3;
ERROR4:  // setPower problem
	gotoxy(5,5);
	printf("error while setting a port\n Please restart ");
	ret->finished=true;
	ret->errorcode=-4;
	return -4;
ERROR5: // placeholder 
	gotoxy(5,5);
	printf("Placeholder\n Please restart ");
	ret->finished=true;
	return -5;
}

int countdown(RACE *ret) {

	int i=0;
	clock_t timer1;

	timer1=clock();
	while(i<4) {   //As long as 3 seconds (for every step-1 (4-1 LEDs) one second )

		ret->startTime = clock();

		if(! ((ret->startTime-timer1)/CLOCKS_PER_SEC < 1) )   //If one second has passed
		{
			timer1=clock();  //Reset CurrentTime
			ret->device.trafficLightStatus=i;  //set current status

			if( !(setLights(& ret->device)==0) )  //Setting the LEDs of the traffic lights
			{
				printf("error while setting a port\n Please restart ");
				return -1;
			}
			i++;
			if(i==3)  //after 3 seconds the game has officially started
				ret->started=true;
		}
	}
	return 0;
}
