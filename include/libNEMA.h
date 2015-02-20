#ifndef __LIB_NEMA
#define __LIB_NEMA

struct __gps_state;
typedef struct __gps_state GpsState;
struct __gps_state{
	float Lat;
	float Lon;
	float Altitude;
	float Speed;
	unsigned char Hour, Minute, Second;
	unsigned char Satellites;
	int  Fix;
	float HDOP;      // Hosizontal dilution of precision
};

extern int FD_GPS;

int  lnConnect (const char* dev, int baud);
int  lnGenState(GpsState* state, void (*UpdateCalback)(GpsState*));
int  lnReadMsg (char* dst, int size);
int  lnSendMsg (char* src, int size);
int  lnParseMsg(GpsState* state, char* msg);
void lnPrintState(GpsState* state);

#endif
