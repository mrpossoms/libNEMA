#include "libNEMA.h"
#include <stdio.h>

void lnPrintState(GpsState* state){
	printf("Position: %fN, %fW\n", state->Lat, state->Lon);
	printf("Altitude: %f\n", state->Altitude);
	printf("Speed: %f\n", state->Speed);
	printf("Update Time: %2d:%2d:%2d\n",
		state->Hour,
		state->Minute,
		state->Second
	);
	printf("Satellites: %d\n", state->Satellites);
	printf("HDOP: %f\n", state->HDOP);
}
