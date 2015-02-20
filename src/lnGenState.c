#include "libNEMA.h"
#include <stdlib.h>
#include <strings.h>

int lnGenState(GpsState* state, void (*UpdateCallback)(GpsState*)){
	GpsState* s = (GpsState*)malloc(sizeof(GpsState));

	bzero(s, sizeof(GpsState));
	return 0;
}
