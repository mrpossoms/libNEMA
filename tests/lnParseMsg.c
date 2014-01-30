#include "libNEMA.h"
#include "test.h"
#include <string.h>
#include <strings.h>

char* GPGLL_TEST = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\0";
char* GPGAA_TEST = "$GPGGA,065500.00,2447.65027,N,12100.78318,W,2,12,0.91,69.8,M,16.3,M,,*65\0";

int main(void){
	char buf[1024];
	GpsState state = {0};
	
	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	__msg("Connecting...");
	int fd = lnConnect("/dev/ttyAMA0", 9600);
	int size = 0, i = 0;
	
	lnGenState(&state, NULL);
	
	memcpy(buf, GPGAA_TEST, strlen(GPGAA_TEST));
	printf("%s\n", buf);
	lnParseMsg(&state, buf);
	lnPrintState(&state);
	printf("\n");

	// assert date
	assert(state.Hour == 06);
	assert(state.Minute == 55);
	assert(state.Second == 00);

	// assert coordinates
	assert((int)state.Lat == 24);
	assert((int)state.Lon == 121);
	assert((int)state.Altitude == 69);

	// assert fix state
	assert(state.Fix == 2);
	assert(state.Satellites == 12);
	assert(state.HDOP == 0.91f);


	memcpy(buf, GPGLL_TEST, strlen(GPGLL_TEST));
	printf("%s\n", buf);
	lnParseMsg(&state, buf);
	printf("\n");
	lnPrintState(&state);
	printf("\n");
	
	// assert coordinates
	assert((int)state.Lat == 49);
	assert((int)state.Lon == 123);

	sleep(1);

	while(size = lnReadMsg(buf, 1024)){
		int i = 0;
		write(1, buf, size);
		write(1, "\n", 1);
		lnParseMsg(&state, buf);
		sleep(1);
		bzero(buf, 1024);

		lnPrintState(&state);
	}

	//assert(lnReadMsg(buf, 1024) > 0);
	printf("len: %d\n", size);
	printf("%s\n", buf);

	assert(!lnParseMsg(&state, buf));
	return 0;
}
