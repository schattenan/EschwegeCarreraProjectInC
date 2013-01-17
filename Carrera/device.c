#include "device.h"



void initDEVICE(DEVICE *ret) {
	int i=0;
	startConnectionUE9(&ret->ue9);

	for(i=0;i<4;i++)
	{
		ret->activeTrack[i]=false;
		ret->activeTrackSensor[i]=false;
	}
	ret->trafficLightStatus=4;
	setLights(ret);
	setPower( ret,0,false );  
	setPower( ret,1,false );  
	setPower( ret,2,false );  
	setPower( ret,3,false );  
	
}

void setLights( DEVICE *ret) {

	if(ret->trafficLightStatus==0) //First red LED
	{
		setPortUE9(&ret->ue9, 0, 1);
	}
	if(ret->trafficLightStatus==1) //Second red LED
	{
		setPortUE9(&ret->ue9, 1, 1);
	}
	if(ret->trafficLightStatus==2) //Third red LED
	{
		setPortUE9(&ret->ue9, 2, 1);
	}
	if(ret->trafficLightStatus==3) //All red LED off, green LED on
	{
		setPortUE9(&ret->ue9,  0, 0);
		setPortUE9(&ret->ue9,  1, 0);
		setPortUE9(&ret->ue9,  2, 0);
		setPortUE9(&ret->ue9,  3, 1);
	}
	if(ret->trafficLightStatus==4)   //ALL Lights OFF, it's getting dark
	{
		setPortUE9(&ret->ue9,  0, 0);
		setPortUE9(&ret->ue9,  1, 0);
		setPortUE9(&ret->ue9,  2, 0);
		setPortUE9(&ret->ue9,  3, 0);
	}
}

void playerCrossesLine(DEVICE *ret, int track) {

	// --------- Check  Port for Digital High ---------
	int i=-1;
	getPortUE9(&ret->ue9,track+8,&i);  //Gets port

	if(i>0)  //If active 
		ret->activeTrackSensor[track]=true;	
	else  //If inactive
		ret->activeTrackSensor[track]=false;
}

void setPower(DEVICE *ret, int track, bool energy ) {

	if(energy)  //power track on
	{
		ret->activeTrack[track] = 1;
		setPortUE9(&ret->ue9, (track+4+8), 1);  // see comments 
	}
	
	if(!energy) //power track off
	{
		ret->activeTrack[track] = 0;
		setPortUE9(&ret->ue9, (track+4+8), 0); // see comments 
	}
}

void startConnectionUE9 (CONNECTION *ret) {

	ret->ue9_port = DEFAULTPORT;

	//Opening TCP connection to UE9
	 if( !( (ret->socketFD = openTCPConnection(DEFAULTIP, ret->ue9_port)) < 0)) //retunrs true if succesfull, native UE9 
			printf("Connected ");

	 //Getting calibration information from UE9  (Necessary for communication)
	 if(! (getCalibrationInfo(ret->socketFD, &ret->caliInfo) < 0))
			printf("\nCalibrated \n");
}

void setPortUE9(CONNECTION *ret, int portNumber, int value) {     

	/* 
		0-7    FIO0-FIO7, 0-3	traffic lights 
		8-11	Sensors 
		12-15	tracks 
		8-15   EIO0-EIO7
		
	*/

	if((ret->error = eDO(ret->socketFD, portNumber, value)) != 0) // Reads port, return true if succesfull, native UE9 
		printf("setPort unsuccesfull");

}

void getPortUE9(CONNECTION *ret, int portNumber, int *value) {
	
	
	/* 
		0-7    FIO0-FIO7, 	0-3	traffic lights
		8-11	Sensors 
		12-15	tracks 
		8-15   EIO0-EIO7
	 
	*/


	if((ret->error = eDI(ret->socketFD, portNumber, &ret->lngState)) != 0)  // Set port, retunrs true if succesfull, native UE9 
		printf("Could not read the state of Port %d", portNumber);


	*value = (int) ret->lngState;

}

void closeConnectionUE9 (CONNECTION *ret) {
  
  if(ret->error > 0)  //Connection issue
    printf("Received an error code of %ld\n", ret->error);
  if(closeTCPConnection(ret->socketFD) < 0)  // closes conncetion, retunrs true if succesfull, native UE9 
  {
    printf("Error: failed to close socket\n");
  }

}
