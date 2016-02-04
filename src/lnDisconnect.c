#include "libNEMA.h"
#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <sys/ioctl.h>

int lnDisconnect(){
  if(tcsetattr(LN_FD_GPS, 0, &LN_ORIG_TOPTIONS) < 0) {
		perror("serialport_init: Couldn't get term attributes");
		return -1;
	}

  if(close(LN_FD_GPS)){
    return -2;
  }

  return 0;
}
