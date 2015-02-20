#include "libNEMA.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

//#define DEBUG

#define GPGGA 0
#define GPGLL 1
#define GPGSA 2
#define GPGSV 3
#define GPRMC 4
#define GPVTG 5
#define GPTXT 6

#define MIN2DEG 0.0166666667f

#define START_PARSE() char checksum = 0;\
	char lastToken[32] = {0};\
	char* token;\
	token = strtok(msgData, ",");\
	do{\

#define CONTINUE_PARSE() bzero(lastToken, 32);\
	memcpy(lastToken, token, strlen(token));\
	} while((token = strtok(NULL, ",")));\

/*---------------------------------------------------------------------------*/
int __is_checksum_valid(int checksum, char* sumStr){
	int parsedSum = 0;
	sscanf(sumStr, "%x", &parsedSum);

#ifdef DEBUG
	printf("scanned sum: %x, calced: %x\n", parsedSum, checksum);
#endif

	return parsedSum == checksum;
}
/*---------------------------------------------------------------------------*/
int __calc_checksum(char* msg, char** sumStr){
	int c = 0, i = 1;

	/* consume string, until the next char is the '*'
	   or the beginning of the sum */
	for(; msg[i] != '*'; i++){
#ifdef DEBUG
		printf("%c ", msg[i]);
#endif

		c ^= msg[i];
	}		

#ifdef DEBUG
	printf("\n%c %c\n", msg[0], msg[2]);
#endif

	/* retain the sum string */
	*sumStr = &msg[i + 1];

	return c;
}
/*---------------------------------------------------------------------------*/
int __GGA(struct GpsHandler* hndlr, char* msgData){
	int timeDone = 0;
	int qualityDone = 0;
	int HDOPDone = 0;
	int satellitesDone = 0;
	int altitudeDone = 0;
	int latDone = 0, lonDone = 0;

	START_PARSE()

	if(!timeDone){
		char  hour[3] = {0};
		char  min[3] = {0};
		char* sec;
		float seconds = 0;
		int minute, hr;

		memcpy(hour, token, 2);
		memcpy(min, token + 2, 2);
		sec = token + 4;

		sscanf(sec,  "%f", &seconds);
		sscanf(min,  "%d", &minute);
		sscanf(hour, "%d", &hr);

		hndlr->state.Second = (unsigned char)seconds;
		hndlr->state.Minute = (unsigned char)minute;
		hndlr->state.Hour   = (unsigned char)hr;

		timeDone = 1;
	}
	else if(!strcmp(token, "N")){
		char* min = lastToken + 2;
		char  deg[3] = {0};
		float minutes = 0;

		memcpy(deg, lastToken, 2);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &hndlr->state.Lat);
#ifdef DEBUG
		printf("Token='%s' lastToken='%s'\n", token, lastToken);
#endif

		hndlr->state.Lat += (MIN2DEG * minutes);
		latDone = 1;
	}
	else if(!strcmp(token, "W")){
		char* min = lastToken + 3;
		char  deg[4] = {0};
		float minutes = 0;

		memcpy(deg, lastToken, 3);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &hndlr->state.Lon);
#ifdef DEBUG
		printf("Token='%s' lastToken='%s'\n", token, lastToken);
#endif

		hndlr->state.Lon += (MIN2DEG * minutes);
		lonDone = 1;
	}
	else if(!latDone || !lonDone){
		bzero(lastToken, 32);
		memcpy(lastToken, token, strlen(token));
		continue;
	}
	else if(!qualityDone){
		sscanf(token, "%d", &hndlr->state.Fix);
		qualityDone = 1;
	}
	else if(!satellitesDone){
		int satillites;
		sscanf(token, "%d", &satillites);
		hndlr->state.Satellites = satillites;
		satellitesDone = 1;
	}
	else if(!HDOPDone){
		sscanf(token, "%f", &hndlr->state.HDOP);
		HDOPDone = 1;
	}
	else if(!altitudeDone){
		sscanf(token, "%f", &hndlr->state.Altitude);
		altitudeDone = 1;
	}
	
	CONTINUE_PARSE()
	return 0;
}
/*---------------------------------------------------------------------------*/
int __GLL(struct GpsHandler* hndlr, char* msgData){
	START_PARSE()
		if(!strcmp(token, "N")){
			char* min = lastToken + 2;
			char  deg[3] = {0};
			float minutes = 0;

			memcpy(deg, lastToken, 2);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &hndlr->state.Lat);

			hndlr->state.Lat += (MIN2DEG * minutes);
		}

		if(!strcmp(token, "W")){
			char* min = lastToken + 3;
			char  deg[4] = {0};
			float minutes = 0;
	
			memcpy(deg, lastToken, 3);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &hndlr->state.Lon);

			hndlr->state.Lon += (MIN2DEG * minutes);
		}
	CONTINUE_PARSE()

	return 0;
}
/*---------------------------------------------------------------------------*/
int lnParseMsg(struct GpsHandler* hndlr, char* msg){
	char msgType[6];
	char* msgData = NULL, *checksumStr = NULL;
	int msgIndex = -1;
	int checksum = __calc_checksum(msg, &checksumStr);

	/* make sure this message isn't crap */
	if(!__is_checksum_valid(checksum, checksumStr)){
		return -1;
	}

	/* split the message header and the data */
	bzero(msgType, sizeof(msgType));
	memcpy((char*)msgType, &msg[1], 5);
	msgData = &msg[6];

	if(!strcmp(msgType, "GPGGA")) msgIndex = GPGGA;
	if(!strcmp(msgType, "GPGLL")) msgIndex = GPGLL;
	if(!strcmp(msgType, "GPGSA")) msgIndex = GPGSA;
	if(!strcmp(msgType, "GPGSV")) msgIndex = GPGSV;
	if(!strcmp(msgType, "GPRMC")) msgIndex = GPRMC;
	if(!strcmp(msgType, "GPVTG")) msgIndex = GPVTG;
	if(!strcmp(msgType, "GPTXT")) msgIndex = GPTXT;

	switch(msgIndex){
		case GPGGA:
			__GGA(hndlr, msgData);
			break;
		case GPGLL:
			__GLL(hndlr, msgData);
			break;
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

#ifdef DEBUG
	printf("%s\n%s\n", msgType, msgData);
#endif

	return 0;
}
