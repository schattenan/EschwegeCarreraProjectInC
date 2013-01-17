
#include "export.h"


int main() {

	char f;
	RACE objectRace;
	
	initRACE(&objectRace);
	printf("\nBuild : %s \n",BUILD);
	
	initUI(&objectRace);
	run(&objectRace);
	exportCSV(&objectRace);
	
	closeConnectionUE9(&objectRace.device.ue9);

	f = getch();
	return 0;
	
}