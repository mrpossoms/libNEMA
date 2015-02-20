#include "libNEMA.h"
#include <stdlib.h>
#include <strings.h>

int lnGenState(struct GpsHandler* state, void (*UpdateCallback)(GpsState*)){
	struct GpsHandler* s = (struct GpsHandler*)malloc(sizeof(GpsState));

	bzero(s, sizeof(struct GpsHandler));
	s->OnUpdate = UpdateCallback;

	return 0;
}
