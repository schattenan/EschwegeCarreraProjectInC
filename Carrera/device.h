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

int initDEVICE(DEVICE *ret);
/*

The function initiates the Device Struct

return value:
	0 - everything is working
   -1 - error, see function startConnectionUE9
   -2 - error, see function setLights

*/

int setLights( DEVICE *ret);
/*

The function sets the LEDs of the traffic light

return value:
	0 - everything is working
   -1 - error, see function setPort


*/

int playerCrossesLine(DEVICE *ret, int track);
/*

The function checks the status of a specific track
track - Number of the player/track

Writes result in struct DEVICE.activeTrackSensor[track]

return value
	 0 - everything is working perfectly
	-1 - error while getting port status

*/

int setPower(DEVICE *ret, int track, bool energy );
/*

The function is setting the power off or on on a specific track
track - Number of the player/track
energy -
		1) if true it turns the power on
		2) if false it turns the power off

portnumber:
		0-7    FIO0-FIO7
		8-15   EIO0-EIO7
		
		0-3	traffic lights
		8-11	Sensors 
		12-15	tracks 
		

return value
	 0 - everything is working perfectly
	-1 - error, see function setPort

*/

int startConnectionUE9 (CONNECTION *ret);
/*

The function initiates the connection to the UE9

return value
	 0 - everything is perfect
	-1 - no socket
	-2 - no calibration

*/

int setPortUE9(CONNECTION *ret, int portNumber, int value);
/*

The function sets one port of the UE9
portNumber - Number of the specific port
value - 1 for on, 0 for off

portnumber:
		0-7    FIO0-FIO7
		8-15   EIO0-EIO7
		
		0-3	traffic lights
		8-11	Sensors 
		12-15	tracks 

return value
	-1 - error while setting a port
	 0 - everything is working fine

*/

int getPortUE9(CONNECTION *ret, int portNumber, int *value);
/*

The function recieves the status of one port 
portNumber - Number of the specific port
value - int pointer for writing the status, 1 for on, 0 for off

return value
	-1 - error getting port status
	 0 - everything is fine
*/

void closeConnectionUE9 (CONNECTION *ret);
/*

The function stopps the connection to the UE9

*/

