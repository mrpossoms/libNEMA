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
	while(buf[0] != '$') read(FD_GPS, &buf[0], 1);
	++i;
	
	// read the contents of a sentence
	while(buf[i] != '\n' && i < size - 1)
		read(FD_GPS, &buf[++i], 1); 

	return i;
}
