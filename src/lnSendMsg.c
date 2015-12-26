#include "libNEMA.h"
#include <stdio.h>    // Standard input/output definitions 
#include <unistd.h>

int lnSendMsg(const char* src, int size){
	return write(FD_GPS, src, size);
}