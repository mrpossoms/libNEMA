#include "libNEMA.h"
#include <stdio.h>    // Standard input/output definitions 

int lnSendMsg(char* src, int size){
	return write(FD_GPS, src, size);
}