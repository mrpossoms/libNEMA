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

#define MIN2DEG 0.0166666667f
int __GLL(char* msgData){
	char checksum = 0;
	char lastToken[32];
	char* token;
	float last = 0;
	float alt = 0;
	float lat = 0, lon = 0;			

	token = strtok(msgData, ",");

	do{
		printf("tok: %s float: %f\n", token, last);

		if(!strcmp(token, "N")){
			char* min = lastToken + 2;
			char  deg[3] = {0};
			float minutes = 0;
	
			memcpy(deg, lastToken, 2);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lat);

			state->Lat += (MIN2DEG * minutes);

			printf("Lat: %f\n", state->Lat);
		}

		if(!strcmp(token, "W")){
			char* min = lastToken + 3;
			char  deg[4] = {0};
			float minutes = 0;
	
			memcpy(deg, lastToken, 3);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lon);

			state->Lon += (MIN2DEG * minutes);

			printf("Lon: %f\n", state->Lon);
		}

		memcpy(lastToken, token, strlen(token));
	} while(token = strtok(NULL, ","));

	return 0;
}

int lnParseMsg(GpsState* state, char* msg){
	char msgType[6];
	char* msgData = NULL;
	int i_msgType = -1;
	bzero(msgType, sizeof(msgType));

	write(1, "Parse: ", 7);
	write(1, msg, strlen(msg));
	write(1, "\n", 1);
	
	memcpy((char*)msgType, &msg[1], 5);
	msgData = &msg[6];


	if(!strcmp(msgType, "GPGGA")) i_msgType = GPGGA;
	if(!strcmp(msgType, "GPGLL")) i_msgType = GPGLL;
	if(!strcmp(msgType, "GPGSA")) i_msgType = GPGSA;
	if(!strcmp(msgType, "GPGSV")) i_msgType = GPGSV;
	if(!strcmp(msgType, "GPRMC")) i_msgType = GPRMC;
	if(!strcmp(msgType, "GPVTG")) i_msgType = GPVTG;
	if(!strcmp(msgType, "GPTXT")) i_msgType = GPTXT;

	printf("Parse: %s, %d\n", msgType, i_msgType); 
	
	switch(i_msgType){
		case GPGGA:
			break;
		case GPGLL:
		{
			__GLL(msgData);
			//printf("lat: %f\nlon: %f\n", state->Lat, state->Lon);
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
