//Author: LabJack
//Oct. 8, 2007
//This examples demonstrates how to read from analog inputs (AIN) and digital inputs(FIO),
//set analog outputs (DAC) and digital outputs (FIO), and how to configure and enable
//timers and counters and read input timers and counters values using the "easy" functions.
#include "connection.h"



void startConnectionUE9 (CONNECTION *ret) {

	ret->ue9_port = DEFAULTPORT;

	//Opening TCP connection to UE9
	 if( !( (ret->socketFD = openTCPConnection(DEFAULTIP, ret->ue9_port)) < 0))
			printf("Connected ");

	 //Getting calibration information from UE9
	 if(! (getCalibrationInfo(ret->socketFD, &ret->caliInfo) < 0))
			printf("\nCalibrated \n");
}

void setPortUE9(CONNECTION *ret, int portNumber, int value) {     


	//printf("SEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET");
	if((ret->error = eDO(ret->socketFD, portNumber, value)) != 0)
		printf("setPort unsuccesfull");
	//printf("PortStatus of %d: %f ",portNumber, value);

}

void getPortUE9(CONNECTION *ret, int portNumber, int *value) {
	
	
	/* 
	0-7    FIO0-FIO7
		8-11	Sensors 
		12-15	tracks 
	8-15   EIO0-EIO7
		0-3	traffic lights 
	*/

	 //Read state of FIO2

	//printf("\nCalling eDI to read the state of FIO2\n");
	if((ret->error = eDI(ret->socketFD, portNumber, &ret->lngState)) != 0)
		printf("Could not read the state of Port %d", portNumber);

	//printf("  %u ",ret->lngState  );
	*value = (int) ret->lngState;
	//printf("PortStatus of %d: %f ",portNumber, ret->lngState);
}

void closeConnectionUE9 (CONNECTION *ret) {
  
  if(ret->error > 0)
    printf("Received an error code of %ld\n", ret->error);
  if(closeTCPConnection(ret->socketFD) < 0)
  {
    printf("Error: failed to close socket\n");
  }

}
