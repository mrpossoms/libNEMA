#include "libNEMA.h"
#include <strings.h>
#include <errno.h>

int lnReadMsg(char* buf, int size){
	int i = 0;

	bzero(buf, sizeof(buf));

	if(!FD_GPS){
		errno = EBADF;
		return -1;
	}

	// wait for the beginning of the message
	while(buf[0] != '$')
		i = read(FD_GPS, buf, size);

	return i;
}
