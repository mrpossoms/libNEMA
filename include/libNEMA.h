#ifndef __LIB_NEMA
#define __LIB_NEMA

struct __gps_state;
typedef struct __gps_state GpsState;
struct __gps_state{
	float Lat;
	float Lon;
	float Speed;
	void (*OnUpdate)(GpsState*);
};

extern int FD_GPS;

int lnConnect (const char* dev, int baud);
int lnGenState(GpsState* state, void (*UpdateCalback)(GpsState*));
int lnReadMsg (char* dst, int size);
int lnParseMsg(GpsState* state, char* msg);

#endif
