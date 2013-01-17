#include "device.h"
#include "player.h"
#include <string.h>
#include <Windows.h>

struct RACE{
         
        bool finished;
        bool started;
        PLAYER players[4];
        DEVICE device;   
		int maxRounds; 
		bool match_Active;
		bool knockOut_Active;
		bool timeAttack_Active;
		clock_t maxTime;
		clock_t currentTime;
		int knockOutPlayer; 
		int knockOutPlayerX[4]; 
		int playerLine;
		int numberOfPlayers;
		bool activeSensor[4];      
};

typedef struct RACE RACE;


void initRACE(RACE *ret);
/*

The function initiates a RACE struct and opens a tcp conncetion to the UE9

*/

void match (RACE *ret);
/*

The function is the logic for the Game Mode race, it checks for specific conditions 

*/

void knockOut (RACE *ret);
/*

The function is the logic for the Game Mode knockout, it checks for specific conditions 

*/

void placingTimeAttack(RACE ret);
/*

The function is called after a game of 'Time Attack' has been finished.
It sets the ranking for all players after the number of finished rounds

*/

void run (RACE *ret);
/*

The function is the main function for a race.
In the beginning the countdown is started and 
afterwards this function starts the threads for 
	1)recieving and computing the race data
	2)updating the UI

afterwards 
*/

void gotoxy(int x, int y);
/*

Generic Win32-APi call for setting the Cursor in a CMD
X - Position in line
Y - Line

*/

bool confirm();
/*

The function is waiting for the users keyboard input. 
If the user types 'y' or 'Y' it will return true
If the user types 'n' or 'N' it will return false

*/

void initUI(RACE *ret);
/*

The function initiates the UI as well as asking the user for information:
Game Mode
Number of Players
Players Name
Number of Rounds
Max. Time

*/

void getName(char name[42]);
/*

The function is asking for a players name

*/

void buildTable(RACE *ret); 
/*

The function is dynamically building a time table 

*/

DWORD WINAPI updateTime(LPVOID data);
/*

The function is updating the times in the time table
returns 0

*/

void printTime(RACE *ret, int player, bool total);
/*

The function is printing the time of the current round or for the total time for specific player

player - The number of the player
total - 
		1) if true it prints the total time
		2) if false it prints the current round time

*/

DWORD WINAPI raceloop(LPVOID data);
/*

The function is recieving the sensor status for every single player and aftwards computes them based on the Game Mode
returns 0

*/

void countdown(RACE *ret);

/*

The function is starting the countdown for the race

*/
