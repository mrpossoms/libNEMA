#include "libNEMA.h"
#include "test.h"

int main(void){
	__title("lnConnect()");
	
	__msg("Connecting...");
	int fd = lnConnect("/dev/ttyAMA0", 9600);
	__msg("FD = %d", fd);
	assert(fd >= 0);
	
	return 0;
}
