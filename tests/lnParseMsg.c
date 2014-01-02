#include "libNEMA.h"
#include "test.h"

int main(void){
	char buf[1024];
	GpsState state;
	
	__title("lnParseMsg()");
	bzero(buf, 1024);
	
	__msg("Connecting...");
	int fd = lnConnect("/dev/ttyAMA0", 9600);
	int size = 0;
	
	lnGenState(&state, NULL);

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
