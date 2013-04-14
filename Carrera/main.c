
#include "export.h"


int main() {

	char f;
	RACE objectRace;
	
	printf("Build : %s \n\n",BUILD);

	if( initRACE(&objectRace) == 0)
	{
		initUI(&objectRace);
		if( !(run(&objectRace)==0 ) )
		{
			exportCSV(&objectRace);
			closeConnectionUE9(&objectRace.device.ue9);
		}

	}else{
		
		printf("\nThere has been an error while trying to connect to the UE9. \nPlease check your connection!");
	}

	f = getch();
	return 0;
	
}