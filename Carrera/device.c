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
	getPortUE9(&ret->ue9,track+8,&i);
	//printf("\n SCAM %d, Status: %d",track+8,i);

	if(i>0)
		ret->activeTrackSensor[track]=true;	
	else
		ret->activeTrackSensor[track]=false;


}

void setPower(DEVICE *ret, int track, bool energy ) {

	if(energy)
	{
		ret->activeTrack[track] = 1;
		setPortUE9(&ret->ue9, (track+4+8), 1);
	}
	
	if(!energy)
	{
		ret->activeTrack[track] = 0;
		setPortUE9(&ret->ue9, (track+4+8), 0);
	}
}




