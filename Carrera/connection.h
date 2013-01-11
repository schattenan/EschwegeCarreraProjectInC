#include "ue9.h"
#include "definitions.h"

typedef struct {
	int socketFD, i;
	ue9CalibrationInfo caliInfo;
	long error;
	long lngState;
	int ue9_port; 
  
}CONNECTION;

void startConnectionUE9 (CONNECTION *ret);
void setPortUE9(CONNECTION *ret, int portNumber, int value);
void getPortUE9(CONNECTION *ret, int portNumber, int *value);
void closeConnectionUE9 (CONNECTION *ret);
