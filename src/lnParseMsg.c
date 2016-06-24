#include "libNEMA.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <math.h>

//#define DEBUG

#define MIN2DEG 0.0166666667f

#define START_PARSE() char checksum = 0;\
	char lastToken[32] = {0};\
	char* token;\
	token = strtok(msgData, ",");\
	do{\

#define IS_TOKEN(str) (!strcmp(token, (str)))
#define IS_LAST_TOKEN(str) (!strcmp(lastToken, (str)))

#define CONTINUE_PARSE() bzero(lastToken, 32);\
	memcpy(lastToken, token, strlen(token));\
	} while((token = strtok(NULL, ",")) != NULL);\

struct ln_nip {
	const char* name;
	int(*procsessor)(gpsState_t*, char*);
	int messageCode;
};

/*---------------------------------------------------------------------------*/
static int is_checksum_valid(int checksum, char* sumStr){
	int parsedSum = 0;
	sscanf(sumStr, "%x", &parsedSum);

#ifdef DEBUG
	printf("scanned sum: %x, calced: %x\n", parsedSum, checksum);
#endif

	return parsedSum == checksum;
}
/*---------------------------------------------------------------------------*/
static int calc_checksum(char* msg, char** sumStr){
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
static int calc_buf_checksum(void* buf, size_t bufLen){
	int c = 0;

	/* consume string, until the next char is the '*'
		or the beginning of the sum */
	for(;bufLen--;){
		c ^= ((unsigned char*)buf)[bufLen];
	}

	return c;
}
/*---------------------------------------------------------------------------*/
static int GGA(gpsState_t* state, char* msgData){
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

		state->Second = (unsigned char)seconds;
		state->Minute = (unsigned char)minute;
		state->Hour   = (unsigned char)hr;

		timeDone = 1;
	}
	else if(IS_TOKEN("N") || IS_TOKEN("S")){
		char* min = lastToken + 2;
		char  deg[3] = {0};
		float minutes = 0;

		memcpy(deg, lastToken, 2);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &state->Lat);
#ifdef DEBUG
		printf("Token='%s' lastToken='%s'\n", token, lastToken);
#endif

		state->Lat += (MIN2DEG * minutes);

		// negate the coordinate if in southern hemisphere
		if(IS_TOKEN("S")){
			state->Lat *= -1;
		}

		latDone = 1;
	}
	else if(IS_TOKEN("W") || IS_TOKEN("E")){
		char* min = lastToken + 3;
		char  deg[4] = {0};
		float minutes = 0;

		memcpy(deg, lastToken, 3);
		sscanf(min, "%f", &minutes);
		sscanf(deg, "%f", &state->Lon);
#ifdef DEBUG
		printf("Token='%s' lastToken='%s'\n", token, lastToken);
#endif

		state->Lon += (MIN2DEG * minutes);

		// negate the coordinate if in the western hemisphere
		if(IS_TOKEN("W")){
			state->Lon *= -1;
		}

		lonDone = 1;
	}
	else if(!latDone || !lonDone){
		bzero(lastToken, 32);
		memcpy(lastToken, token, strlen(token));
		continue;
	}
	else if(!qualityDone){
		sscanf(token, "%d", &state->Fix);
		qualityDone = 1;
	}
	else if(!satellitesDone){
		int satillites;
		sscanf(token, "%d", &satillites);
		state->Satellites = satillites;
		satellitesDone = 1;
	}
	else if(!HDOPDone){
		sscanf(token, "%f", &state->HDOP);
		HDOPDone = 1;
	}
	else if(!altitudeDone){
		sscanf(token, "%f", &state->Altitude);
		altitudeDone = 1;
	}

	CONTINUE_PARSE()
	return 0;
}
/*---------------------------------------------------------------------------*/
static int GLL(gpsState_t* state, char* msgData){
	START_PARSE()
		if(IS_TOKEN("N") || IS_TOKEN("S")){
			char* min = lastToken + 2;
			char  deg[3] = {0};
			float minutes = 0;

			memcpy(deg, lastToken, 2);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lat);

			state->Lat += (MIN2DEG * minutes);

			// negate the coordinate if in southern hemisphere
			if(IS_TOKEN("S")){
				state->Lat *= -1;
			}
		}

		if(IS_TOKEN("W") || IS_TOKEN("E")){
			char* min = lastToken + 3;
			char  deg[4] = {0};
			float minutes = 0;

			memcpy(deg, lastToken, 3);
			sscanf(min, "%f", &minutes);
			sscanf(deg, "%f", &state->Lon);

			state->Lon += (MIN2DEG * minutes);

			// negate the coordinate if in the western hemisphere
			if(IS_TOKEN("W")){
				state->Lon *= -1;
			}
		}
	CONTINUE_PARSE()

	return 0;
}
/*---------------------------------------------------------------------------*/
static int VTG(gpsState_t* state, char* msgData){
	START_PARSE()
	if(IS_TOKEN("T")){ // true track, made good
		sscanf(lastToken, "%f", &state->Bearing);
		state->Bearing *= (M_PI / 180.0f); // convert to radians
	}
	else if(IS_TOKEN("K")){
		sscanf(lastToken, "%f", &state->Speed);
	}
	CONTINUE_PARSE()

	return 0;
}
/*---------------------------------------------------------------------------*/
int lnParseMsg(gpsState_t* state, char* msg){
	char msgType[6];
	char* msgData = NULL, *checksumStr = NULL;
	int msgIndex = -1;
	int checksum = calc_checksum(msg, &checksumStr);

	/* make sure this message isn't crap */
	if(!is_checksum_valid(checksum, checksumStr)){
		return -1;
	}

	/* split the message header and the data */
	bzero(msgType, sizeof(msgType));
	memcpy((char*)msgType, &msg[1], 5);
	msgData = &msg[6];

	const struct ln_nip nips[] = {
		{ "GPGGA", GGA,  LN_GPGGA },
		{ "GPGLL", GLL,  LN_GPGLL },
		{ "GPGSA", NULL, LN_GPGSA }, // proc unimplemented
		{ "GPGSV", NULL, LN_GPGSV }, // proc unimplemented
		{ "GPRMC", NULL, LN_GPRMC }, // proc unimplemented
		{ "GPVTG", VTG,  LN_GPVTG },
		{ "GPTXT", NULL, LN_GPTXT }, // proc unimplemented
	};

	// find the correct message type and process
	for(int i = sizeof(nips) / sizeof(struct ln_nip); i--;){
		const struct ln_nip* nip = nips + i;
		if(!strcmp(nip->name, msgType) && nip->procsessor){
			#ifdef DEBUG
				printf("%s\n%s\n", msgType, msgData);
			#endif

			nip->procsessor(state, msgData);
			state->checksum = calc_buf_checksum(state, sizeof(gpsState_t) - sizeof(unsigned char));

			return nip->messageCode;
		}
	}

	return -1;
}
