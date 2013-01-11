#include "modes.h"
        
void initRACE  (RACE *ret) {
         
              ret->finished=false;
              ret->canceled=false;
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
              initDEVICE(& ret->device);
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

}

void match (RACE *ret) {

	nextRound(&ret->players[ret->playerLine],ret->currentTime);


	if(ret->players[ret->playerLine].rounds == ret->maxRounds)
	{
		setPower( &ret->device,(ret->playerLine),false );  //Power off for player 

		ret->knockOutPlayer++;
		ret->players[ret->playerLine].finished=true;
		ret->players[ret->playerLine].endTime=clock();
		ret->players[ret->playerLine].rank = ret->knockOutPlayer;

		 // -------------------  Checks all players  -------------------------- \\ 

		if(ret->knockOutPlayer==ret->numberOfPlayers)
		{
			ret->finished=true;   //race has ended
		}
	}
}

void knockOut (RACE *ret) {   //To-Do 

	int i;

	if(!ret->players[ret->playerLine].finished)
		nextRound(&ret->players[ret->playerLine],ret->currentTime);

	if(ret->players[ret->playerLine].rounds>0) {

		ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]++;

		if(ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]==ret->numberOfPlayers-ret->players[ret->playerLine].rounds+1)
		{
			ret->knockOutPlayerX[ret->players[ret->playerLine].rounds-1]=0;
			ret->knockOutPlayer++; 

			ret->players[ret->playerLine].finished=true;
			ret->players[ret->playerLine].endTime=clock();
			setPower( &ret->device,ret->playerLine,false );
			ret->players[ret->playerLine].rank=ret->numberOfPlayers-ret->players[ret->playerLine].rounds+1;

			if(ret->knockOutPlayer==ret->numberOfPlayers)
				ret->finished=true;	
		}

	}

}

void cancel (RACE *ret) {   //cancel a match
              
              ret->finished=true;
              ret->canceled=true;
              
}

void placingTimeAttack(RACE *ret) {

	int i;
	int runde[4]= {ret->players[0].rounds,ret->players[1].rounds,ret->players[2].rounds,ret->players[3].rounds};
	int win[4]={0,0,0,0};
	int platz[4];
	int j,k=0,m;

	for(i=0;i<ret->numberOfPlayers;i++)  //n*n
		for(j=0;j<ret->numberOfPlayers;j++)
		{
			if(runde[i]>=runde[j])
				win[i]++;
		}
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

void run (RACE *ret) {

	HANDLE hThread[2];	

	countdown(ret);

	if(!ret->started)
	{
		printf("Countdown failed, get used to it");
	}else{
		//Set all tracks active
		if(ret->numberOfPlayers>0){setPower( &ret->device,0,true ); ret->players[0].roundTime[0]=clock(); }
		if(ret->numberOfPlayers>1){setPower( &ret->device,1,true ); ret->players[1].roundTime[0]=clock(); }
		if(ret->numberOfPlayers>2){setPower( &ret->device,2,true ); ret->players[2].roundTime[0]=clock(); }
		if(ret->numberOfPlayers>3){setPower( &ret->device,3,true ); ret->players[3].roundTime[0]=clock(); }

		hThread[0] = CreateThread(NULL,0, raceloop, ret, 0, NULL);
		hThread[1] = CreateThread(NULL,0, updateTime, ret, 0, NULL);

		WaitForMultipleObjects( 2, hThread , true , INFINITE  ); 
		hThread[1] = CreateThread(NULL,0, updateTime, ret, 0, NULL);  // updating UI a last time
		WaitForMultipleObjects( 1, hThread , true , INFINITE  ); 

		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);
	}
	printf("\n\n\nRace has been finished");
	updateTime(ret);
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
		if(a==89 || a ==121)
		{
			system("cls");
			return true;
		}
		
		if(a==78 || a==110)
		{	
			system("cls");
			return false;
		}
	}while(1);
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
		printf("\nWhat kind of Game Mode you want to play?\n\n");
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

	}while(!decision && !test);



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
			else
				printf("\nYou have choosen %d. Please insert valid number (1-99)",ret->maxRounds);
	
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
			if(timeInt>0 && timeInt<9)
			{
				printf("%d Minutes? (y/n)",timeInt);
				test=confirm();
				ret->maxTime = timeInt*60*CLOCKS_PER_SEC;

			}
			else
				printf("\nYou have choosen %d. Please insert valid number (1-9)",ret->maxRounds);
	
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
		printf("Instert the name of the Player(s)\n\n");

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
		printf("\nPlease insert the name\n\n");

		fflush(stdin);
		fgets(name, 20, stdin);

		ln = strlen(name) - 1;
		if (name[ln] == '\n')
			name[ln] = '\0';
		printf("Your name is '%s' ",name);

		a = getch();
}

void buildTable (RACE *ret) 
{
	int i;

	system("cls");

	gotoxy(0,1);
	printf("        I                I                I                I                 ");

	for(i=0;i<ret->numberOfPlayers;i++)
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
	int i;

	RACE *ret;
	ret = (int) data;     // pointer is correct, ignore warning
	
	do{
		for(i=0;i<ret->numberOfPlayers;i++)
		{
			if(!ret->players[i].finished)
			{
				gotoxy(10+i*17,ret->players[i].rounds+3);  
				if(!(ret->players[i].rounds==-1))
					printTime(ret,i,false);

				gotoxy(10+i*17,ret->maxRounds+4);
				printTime(ret,i,true);
			}
			if(ret->players[i].finished)
			{
				gotoxy(10+i*17,ret->maxRounds+5);
				printf("    %d.",ret->players[i].rank);
			}
		}
	}while(!(ret->finished));
	
}

void printTime(RACE *ret, int player, bool total)
{
	int minuten, sekunden, millesekunden;
	clock_t timerX;
	timerX=clock();

		if(total)
		{
			millesekunden = (float) (timerX-ret->currentTime);
			if(ret->players[player].finished)
				millesekunden = (float) ret->players[player].roundTime[ret->players[player].rounds]-ret->players[player].roundTime[0];
		}
		else{			
			millesekunden = (float) (timerX-ret->players[player].roundTime[ret->players[player].rounds]);
			if(ret->players[player].finished)
				millesekunden = (float) ret->players[player].roundTime[ret->players[player].rounds]-ret->players[player].roundTime[ret->players[player].rounds-1];
		}
	
	
	sekunden = millesekunden / 1000;
	minuten = sekunden / 60;
	millesekunden = millesekunden - 1000*sekunden;	
	sekunden = sekunden - 60*minuten;
	printf("%02d:%02d:%03d\n",minuten,sekunden,millesekunden);
}

DWORD WINAPI raceloop(LPVOID data) {
	
	int k;
	clock_t timer1;

	RACE *ret;
	ret = (int) data;     // pointer is correct, ignore warning

	do
	{
		for(k=0; k < ret->numberOfPlayers ;k++)
			{
			playerCrossesLine(& ret->device, k);
				
			if(ret->device.activeTrackSensor[k] && !ret->activeSensor[k] && !ret->players[k].finished)
			{

				ret->playerLine=k;
				ret->activeSensor[k]=true;

				 //Big issue if there are 2 active game modes

				if(ret->match_Active)
					match(ret);
				if(ret->knockOut_Active)
					knockOut( ret);
				if(ret->timeAttack_Active) 
				{
					nextRound(&ret->players[ret->playerLine],ret->currentTime);
					if(ret->players[ret->playerLine].rounds==ret->maxRounds-1)
					{
						ret->maxRounds++;
						buildTable(ret);
					}
				}
			}
			if( (!ret->device.activeTrackSensor[k]) && ret->activeSensor[k])
				ret->activeSensor[k]=false;
			timer1=clock();
			if(  (timer1-ret->currentTime) >  ret->maxTime && ret->timeAttack_Active) //For TimeAttack
			{
				//Turn power off for all players
				setPower( &ret->device,0,false );   
				setPower( &ret->device,1,false );
				setPower( &ret->device,2,false );
				setPower( &ret->device,3,false );
				ret->finished=true;	//race has ended
				ret->players[0].finished=true;
				ret->players[1].finished=true;
				ret->players[2].finished=true;
				ret->players[3].finished=true;
				ret->players[0].endTime=clock();
				ret->players[1].endTime=clock();
				ret->players[2].endTime=clock();
				ret->players[3].endTime=clock();

				placingTimeAttack(ret);
			}
		}
	}while(! (ret->finished) );	

	return 0;
}

void countdown(RACE *ret) {

	int i=0;
	clock_t timer1;

	timer1=clock();
	while(i<4) {

		ret->currentTime= clock();

		if(! ((ret->currentTime-timer1)/CLOCKS_PER_SEC < 1) )  
		{
			timer1=clock();  //Reset CurrentTime
			ret->device.trafficLightStatus=i;  //set current status

			setLights(& ret->device);
			i++;
			if(i==3)
				ret->started=true;
		}
	}
}
