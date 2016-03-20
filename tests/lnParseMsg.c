#include <libNEMA.h>
#include "test.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// checksum is wrong on message below
char* GPGLL_TEST_BAD = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1F\0";
char* GPGLL_TEST = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\0";
char* GPGAA_TEST = "$GPGGA,,,,,,0,00,99.99,,,,,,*48";

void lnParseMsgShouldFailBadCheckSum(void){
	char buf[1024];
	gpsState_t state = {0};
	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	memcpy(buf, GPGLL_TEST_BAD, strlen(GPGLL_TEST_BAD));
	printf("%s\n", buf);
	assert(lnParseMsg(&state, buf) < 0); // should fail
	lnPrintState(&state);
	printf("\n");
}

void lnParseMsgShouldSucceedGPGAA(void){
	gpsState_t state = {0};
	char buf[1024];
	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	memcpy(buf, GPGAA_TEST, strlen(GPGAA_TEST));
	printf("%s\n", buf);
	lnParseMsg(&state, buf);
	lnPrintState(&state);
	printf("\n");

	// assert date
	assert(state.Hour == 0);
	assert(state.Minute == 0);
	assert(state.Second == 0);

	// assert coordinates
	assert((int)state.Lat == 0);
	assert((int)state.Lon == 0);
	assert((int)state.Altitude == 0);

	// assert fix hnd
	assert(state.Fix == 0);
	assert(state.Satellites == 0);
	assert(state.HDOP == 0);
}

void lnParseMsgShouldSucceedGPGLL(){
	gpsState_t state = {0};
	char buf[1024];
	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	memcpy(buf, GPGLL_TEST, strlen(GPGLL_TEST));
	printf("%s\n", buf);
	lnParseMsg(&state, buf);
	printf("\n");
	lnPrintState(&state);
	printf("\n");
	
	// assert coordinates
	assert((int)state.Lat == 49);
	assert((int)state.Lon == -123);
}

int main(int argc, char* argv[]){
	char buf[1024];
	gpsState_t state = {0};
	
	if(argc < 3){
		printf("Usage:\n\t[serial device path] [baud]\n");
	}	


	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	__msg("Connecting...");
	int fd = lnConnect(argv[1], atoi(argv[2]));
	int size = 0, i = 0;

	lnParseMsgShouldSucceedGPGLL();
	lnParseMsgShouldFailBadCheckSum();
	lnParseMsgShouldSucceedGPGAA();

/*
	const char* refreshRate = "$PMTK220,100*2F\r\n";
	size_t len = strlen(refreshRate);
	assert(lnSendMsg(refreshRate, len) == len); 
*/	
	sleep(1);

	while((size = lnReadMsg(buf, 1024))){
		int i = 0;
		//write(1, buf, size);
		write(1, "\nPRSING\n", 8);
		lnParseMsg(&state, buf);
		bzero(buf, 1024);

		lnPrintState(&state);
	}

	//assert(lnReadMsg(buf, 1024) > 0);
	printf("len: %d\n", size);
	printf("%s\n", buf);

	assert(!lnParseMsg(&state, buf));
	return 0;
}
