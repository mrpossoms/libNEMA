#ifndef __LIB_NEMA
#define __LIB_NEMA

typedef struct {
	float Lat;
	float Lon;
	float Altitude;
	float Speed;
	unsigned char Hour, Minute, Second;
	unsigned char Satellites;
	int  Fix;
	float HDOP;      // Hosizontal dilution of precision
	unsigned char checksum;
} gpsState_t;

extern int FD_GPS;

int  lnConnect (const char* dev, int baud);
int  lnReadMsg (char* dst, int size);
int  lnSendMsg (const char* src, int size);
int  lnParseMsg(gpsState_t* hndlr, char* msg);
void lnPrintState(gpsState_t* state);

#endif
