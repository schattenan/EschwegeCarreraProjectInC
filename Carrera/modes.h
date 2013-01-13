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
void match (RACE *ret);
void knockOut (RACE *ret);
void placingTimeAttack(RACE ret);
void run (RACE *ret);

void gotoxy(int x, int y);
bool confirm();
void initUI(RACE *ret);
void getName(char name[42]);
void buildTable(RACE *ret); 
DWORD WINAPI updateTime(LPVOID data);
void printTime(RACE *ret, int player, bool total);
DWORD WINAPI raceloop(LPVOID data);
void countdown(RACE *ret);