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

#define START_PARSE() char checksum = 0;\
	char lastToken[32];\
	char* token;\
	token = strtok(msgData, ",");\
	do{\

#define CONTINUE_PARSE() memcpy(lastToken, token, strlen(token));\
	} while(token = strtok(NULL, ","));\


int __GGA(GpsState* state, char* msgData){
	int timeDone = 0;
	int qualityDone = 0;
	int HDOFDone = 0;
	int satellitesDone = 0;
	int altitudeDone = 0;

	START_PARSE()

	if(!timeDone){
		char  hour[3] = {0};
		char  min[3] = {0};
		char* sec;
		float seconds = 0;

		memcpy(hour, msgData, 2);
		memcpy(min, msgData + 2, 2);
		sec = msgData + 4;

		sscanf(sec,  "%f", &seconds);
		sscanf(min,  "%d", &state->Minute);
		sscanf(hour, "%d", &state->Hour);

		state->Second = (int)seconds;

		timeDone = 1;
	}
	else if(!strcmp(token, "N")){
		char* min = lastToken + 2;
		char  deg[3] = {0};
		float minutes = 0;
	
		memcpy(deg, lastToken, 2);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &state->Lat);

		state->Lat += (MIN2DEG * minutes);
	}
	else if(!strcmp(token, "W")){
		char* min = lastToken + 3;
		char  deg[4] = {0};
		float minutes = 0;
	
		memcpy(deg, lastToken, 3);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &state->Lon);

		state->Lon += (MIN2DEG * minutes);
	}
	else if(!qualityDone){
		// TODO
		qualityDone = 1;
	}
	else if(!satellitesDone){
		sscanf(token, "%d", &state->Satellites);
		satellitesDone = 1;
	}
	else if(!HDOFDone){
		sscanf(token, "%f", &state->HDOP);
		HDOFDone = 1;
	}
	else if(!altitudeDone){
		sscanf(token, "%f", &state->Altitude);
		altitudeDone = 1;
	}
	
	CONTINUE_PARSE()
	return 0;
}

int __GLL(GpsState* state, char* msgData){

	START_PARSE()
		if(!strcmp(token, "N")){
			char* min = lastToken + 2;
			char  deg[3] = {0};
			float minutes = 0;
	
			memcpy(deg, lastToken, 2);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lat);

			state->Lat += (MIN2DEG * minutes);
		}

		if(!strcmp(token, "W")){
			char* min = lastToken + 3;
			char  deg[4] = {0};
			float minutes = 0;
	
			memcpy(deg, lastToken, 3);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lon);

			state->Lon += (MIN2DEG * minutes);
		}

	CONTINUE_PARSE()
	return 0;
}

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
		{

			__GGA(state, msgData);
		} break;
		case GPGLL:
		{
			__GLL(state, msgData);
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
