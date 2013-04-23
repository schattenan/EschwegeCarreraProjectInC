#include "device.h"

int initDEVICE(DEVICE *ret) {
	int i=0;

	if( startConnectionUE9(&ret->ue9) != 0 )
	{
		return -1;
	}

	for(i=0;i<4;i++)
	{
		ret->activeTrack[i]=false;
		ret->activeTrackSensor[i]=false;
	}
	ret->trafficLightStatus=4;
	if(setLights(ret) != 0)
		return -2;
	if( setPower( ret,0,false) !=0)
		return -2;
	if( setPower( ret,1,false) !=0)
		return -2;
	if( setPower( ret,2,false) !=0)
		return -2;
	if( setPower( ret,3,false) !=0)
		return -2;
	
	return 0;
}

int setLights( DEVICE *ret) {

	if(ret->trafficLightStatus==0) //First red LED
	{
		if( !(setPortUE9(&ret->ue9, 0, 1) == 0))
			return -1;
	}
	if(ret->trafficLightStatus==1) //Second red LED
	{
		if( !(setPortUE9(&ret->ue9, 1, 1) == 0) )
			return -1;
	}
	if(ret->trafficLightStatus==2) //Third red LED
	{
		if( !(setPortUE9(&ret->ue9, 2, 1) == 0) )
			return -1;
	}
	if(ret->trafficLightStatus==3) //All red LED off, green LED on
	{
		if( !(setPortUE9(&ret->ue9,  0, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  1, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  2, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  3, 1) == 0) )
			return -1;
	}
	if(ret->trafficLightStatus==4)   //ALL Lights OFF, it's getting dark
	{
		if( !(setPortUE9(&ret->ue9,  0, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  1, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  2, 0) == 0) )
			return -1;
		if( !(setPortUE9(&ret->ue9,  3, 0) == 0) )
			return -1;
	}
	return 0;
}

int playerCrossesLine(DEVICE *ret, int track) {

	// --------- Check  Port for Digital High ---------
	int i=-1;
	if ( !(getPortUE9(&ret->ue9,track+8,&i) == 0 ) )  //Gets port
		return -1;

	if(i>0)  //If active 
		ret->activeTrackSensor[track]=true;	
	else  //If inactive
		ret->activeTrackSensor[track]=false;

	return 0;
}

int setPower(DEVICE *ret, int track, bool energy ) {

	if(energy)  //power track on
	{
		ret->activeTrack[track] = 1;
		if( !(setPortUE9(&ret->ue9, (track+4+8), 1) == 0) )  // see comments 
			return -1;
	}
	
	if(!energy) //power track off
	{
		ret->activeTrack[track] = 0;
		if ( !(setPortUE9(&ret->ue9, (track+4+8), 0) ==0) ) // see comments 
			return -1;
	}
	return 0;
}

int startConnectionUE9 (CONNECTION *ret) {

	ret->ue9_port = DEFAULTPORT;

	printf("Trying to connect to UE9...");

	//Opening TCP connection to UE9
	 if( ( (ret->socketFD = openTCPConnection(DEFAULTIP, ret->ue9_port)) < 0)) //retunrs true if succesfull, native UE9 
	 {
			return -1;
	 }
	 else{
		 //Getting calibration information from UE9  (Necessary for communication)
		if( (getCalibrationInfo(ret->socketFD, &ret->caliInfo) < 0))
		{
				return -2;
		}		
		else{
			return 0; //Everything perfect
		}
	 }
	 
	 
}

int setPortUE9(CONNECTION *ret, int portNumber, int value) {     

	/* 
		0-7    FIO0-FIO7, 0-3	traffic lights 
		8-11	Sensors 
		12-15	tracks 
		8-15   EIO0-EIO7
		
	*/

	if((ret->error = eDO(ret->socketFD, portNumber, value)) != 0) // Reads port, return false if succesfull, native UE9 
		return -1;
	else
		return 0;

}

int getPortUE9(CONNECTION *ret, int portNumber, int *value) {
	
	
	/* 
		0-7    FIO0-FIO7, 	0-3	traffic lights
		8-11	Sensors 
		12-15	tracks 
		8-15   EIO0-EIO7
	 
	*/


	if((ret->error = eDI(ret->socketFD, portNumber, &ret->lngState)) != 0)  // Get port, returns true if succesfull, native UE9 
	{
		printf("Could not read the state of Port %d", portNumber);
		return -1;
	}
	else	
	{
		*value = (int) ret->lngState;
		return 0;
	}

}

void closeConnectionUE9 (CONNECTION *ret) {
  
  if(ret->error > 0)  //Connection issue
    printf("Received an error code of %ld\n", ret->error);
  if(closeTCPConnection(ret->socketFD) < 0)  // closes conncetion, retunrs true if succesfull, native UE9 
  {
    printf("Error: failed to close socket\n");
  }

}
