#include "libNEMA.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

#define GPGGA 0
#define GPGLL 1
#define GPGSA 2
#define GPGSV 3
#define GPRMC 4
#define GPVTG 5
#define GPTXT 6

int lnParseMsg(GpsState* state, char* msg){
	char msgType[6];
	char* msgData = NULL;
	int i_msgType = -1;
	bzero(msgType, sizeof(msgType));
	
	memcpy((char*)msgType, &msg[1], 5);
	msgData = &msg[6];

	if(!strcmp(msgType, "GPGGA")) i_msgType = GPGGA;
	if(!strcmp(msgType, "GPGLL")) i_msgType = GPGLL;
	if(!strcmp(msgType, "GPGSA")) i_msgType = GPGSA;
	if(!strcmp(msgType, "GPGSV")) i_msgType = GPGSV;
	if(!strcmp(msgType, "GPRMC")) i_msgType = GPRMC;
	if(!strcmp(msgType, "GPVTG")) i_msgType = GPVTG;
	if(!strcmp(msgType, "GPTXT")) i_msgType = GPTXT;

	switch(i_msgType){
		case GPGGA:
			break;
		case GPGLL:
		{
			char checksum = 0;
			float alt = 0;
			sscanf(msgData, 
				",%f,N,%f,E,%f,A,D*%2x",
				&state->Lat, &state->Lon,
				&alt, &checksum
			);
			printf("lat: %f\nlon: %f\n", state->Lat, state->Lon);
		} break;
		case GPGSA:
			break;
		case GPGSV:
			break;
		case GPRMC:
			break;
		case GPVTG:
			break;
		case GPTXT:
			break;
	}

	printf("%s\n%s\n", msgType, msgData);

	return 0;
}
