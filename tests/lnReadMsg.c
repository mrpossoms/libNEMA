#include <unistd.h>

#include "libNEMA.h"
#include "test.h"

int main(void){
	char buf[1024];

	__title("lnReadMsg()");
	bzero(buf, 1024);
	
	__msg("Connecting...");
	int fd = lnConnect("/dev/ttyAMA0", 9600);
	int size = 0;

	sleep(1);
	
	assert(lnReadMsg(buf, 1024) > 0);
	printf("len: %d\n", size);
	printf("%s\n", buf);

	return 0;
}
