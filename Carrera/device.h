#include "connection.h"
#include "stdbool.h"

typedef struct {

	bool activeTrack[4];
	bool activeTrackSensor[4];
	int trafficLightStatus;
	CONNECTION ue9;

}DEVICE;

void initDEVICE(DEVICE *ret);
void setLights( DEVICE *ret);
void playerCrossesLine(DEVICE *ret, int track);
void setPower(DEVICE *ret, int track, bool energy );

