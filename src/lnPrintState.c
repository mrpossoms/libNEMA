#include "libNEMA.h"
#include <stdio.h>

char* __FIX[] = {
	"Invalid",
	"Valid",
	"Differential",
	"Not supported",
	"Not supported",
	"Not supported",
	"Dead reckoning"
};

void lnPrintState(gpsState_t* state){
	printf("Position: %f, %f\n", state->Lat, state->Lon);
	printf("Altitude: %f\n", state->Altitude);
	printf("Speed: %f\n", state->Speed);
	printf("Update Time: %02d:%02d:%02d\n",
		state->Hour,
		state->Minute,
		state->Second
	);
	printf("Satellites: %d\n", state->Satellites);
	//printf("Fix status: %s\n", __FIX[state->Fix]);
	printf("HDOP: %f\n", state->HDOP);
}
