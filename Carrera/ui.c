#include "ui.h"

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

void initUI() {

	char a;
	bool decision=false, test;
	int player=0;
	char playername1[42]=("PLAYER1\0");
	char playername2[42]=("PLAYER2\0");
	char playername3[42]=("PLAYER3\0");
	char playername4[42]=("PLAYER4\0");
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
				decision=true;
		}

		if(a==50 ||  a==34)
		{
			printf("\n You want to choose Knock Out? (y/n)"); 
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==51 ||  a==35 ||  a==-11)
		{
			printf("\n You want to choose Time Attack? (y/n)"); 
			test=confirm();
			if( (test) )
				decision=true;
		}

	}while(!decision && !test);


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
			player=1;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==50 ||  a==34)
		{
			printf("\n You want to choose 2 Player? (y/n)"); 
			player=2;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==51 ||  a==35 ||  a==-11)
		{
			printf("\n You want to choose 3 Player? (y/n)"); 
			player=3;
			test=confirm();
			if( (test) )
				decision=true;
		}
		if(a==52 ||  a==36)
		{
			printf("\n You want to choose 4 Player? (y/n)"); 
			player=4;
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

		if(player>0)printf("     1) %s\n",playername1); 
		if(player>1)printf("     2) %s\n",playername2);
		if(player>2)printf("     3) %s\n",playername3);
		if(player>3)printf("     4) %s\n",playername4); 
		printf("\n     5) Continue\n");


		a=getch();
		test=false;

		if(a==49 ||  a==33)
		{
			printf("\n You want to rename %s? (y/n)",playername1); 
			test=confirm();
			if( (test) )
			{
				getName(playername1);
			}
				
		}
		if((a==50 ||  a==34) && player>1) 
		{
			printf("\n You want to rename %s? (y/n)",playername2); 
			test=confirm();
			if( (test) )
			{
				getName(playername2);
			}
		}
		if((a==51 ||  a==35 ||  a==-11) && player>2)
		{
			printf("\n You want to rename %s? (y/n)",playername3); 
			test=confirm();
			if( (test) )
			{
			a	getName(playername3);
			}
		}
		if((a==52 ||  a==36) && player>3)
		{
			printf("\n You want to rename %s? (y/n)",playername4); 
			test=confirm();
			if( (test) )
			{
				getName(playername4);
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

	buildTable(playername1,playername2,playername3,playername4);

}

void getName(char name[42]) {

		char a;
		size_t ln;

		printf("\nWelcome to the BG Carrera Application \n");
		printf("_____________________________________________\n\n\n");
		printf("\nPlease insert the name\n\n");

		fflush(stdin);
		fgets(name, 42, stdin);

		ln = strlen(name) - 1;
		if (name[ln] == '\n')
			name[ln] = '\0';
		printf("Your name is '%s' ",name);

		a = getch();
}

void buildTable (char playername1[42],char playername2[42],char playername3[42],char playername4[42]) 
{
	char z;
	int i;
	int rounds=3;

	gotoxy(10-2,1);
	printf("I %s",playername1);
	gotoxy(27-2,1);
	printf("I %s",playername2);
	gotoxy(27+17-2,1);
	printf("I %s",playername3);
	gotoxy(27+17+17-2,1);
	printf("I %s",playername4);
	gotoxy(0,2);
	printf("--------I----------------I----------------I----------------I-----------------");

	for(i=0;i<rounds;i++)
	{
		gotoxy(0,i+3);
		printf("Lap %02d  I 00:00:000      I  00:00:000     I  00:00:000     I  00:00:000      \n",i+1);
	}
	gotoxy(0,rounds+3);
	printf("--------I----------------I----------------I----------------I-----------------");
	gotoxy(0,rounds+4);
	printf("Total   I 00:00:000      I  00:00:000     I  00:00:000     I  00:00:000      \n");
	gotoxy(0,rounds+5);
	printf("Place   I     1.         I      2.        I      3.        I      4.         \n");

	z=getch();
}
