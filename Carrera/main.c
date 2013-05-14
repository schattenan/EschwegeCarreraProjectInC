
#include "export.h"

int main() {

	char f = getch();
	RACE objectRace;
	
	printf("Build : %s \n\n",BUILD);

	if( initRACE(&objectRace) == 0)
	{
		initUI(&objectRace);
		if( (run(&objectRace)==0 ) )
		{
			exportCSV(&objectRace);
			system(IMPORTPATH);  //Does open the website for printing
			closeConnectionUE9(&objectRace.device.ue9);
		}
	}else{
		gotoxy(0,5);
		printf("There has been an error while trying to connect to the UE9. \nPlease check your connection!");
	}

	f = getch();
	return 0;	
} 
