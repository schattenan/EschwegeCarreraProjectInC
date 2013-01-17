#include "ue9.h"
#include "stdbool.h"
#include "definitions.h"

typedef struct {
	int socketFD, i;
	ue9CalibrationInfo caliInfo;
	long error;
	long lngState;
	int ue9_port; 
  
}CONNECTION;

typedef struct {

	bool activeTrack[4];
	bool activeTrackSensor[4];
	int trafficLightStatus;
	CONNECTION ue9;

}DEVICE;

void initDEVICE(DEVICE *ret);
/*

The function initiates the Device Struct

*/

void setLights( DEVICE *ret);
/*

The function sets the LEDs of the traffic light

*/

void playerCrossesLine(DEVICE *ret, int track);
/*

The function checks the status of a specific track
track - Number of the player/track

Writes result in struct DEVICE.activeTrackSensor[track]

*/

void setPower(DEVICE *ret, int track, bool energy );
/*

The function is setting the power off or on on a specific track
track - Number of the player/track
energy -
		1) if true it turns the power on
		2) if false it turns the power off


*/

void startConnectionUE9 (CONNECTION *ret);
/*

The function initiates the connection to the UE9

*/

void setPortUE9(CONNECTION *ret, int portNumber, int value);
/*

The function sets one port of the UE9
portNumber - Number of the specific port
value - 1 for on, 0 for off

*/

void getPortUE9(CONNECTION *ret, int portNumber, int *value);
/*

The function recieves the status of one port 
portNumber - Number of the specific port
value - int pointer for writing the status, 1 for on, 0 for off

*/

void closeConnectionUE9 (CONNECTION *ret);
/*

The function stopps the connection to the UE9

*/

