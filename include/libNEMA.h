#ifndef __LIB_NEMA
#define __LIB_NEMA

#include <termios.h>  // POSIX terminal control definitions

#ifdef __cplusplus
extern "C" {
#endif

#define LN_GPGGA 0
#define LN_GPGLL 1
#define LN_GPGSA 2
#define LN_GPGSV 3
#define LN_GPRMC 4
#define LN_GPVTG 5
#define LN_GPTXT 6

typedef struct {
	float Lat;
	float Lon;
	float Altitude; // Meters
	float Speed;    // KM / Hr
	float Bearing;  // Radians
	unsigned char Hour, Minute, Second;
	unsigned char Satellites;
	int  Fix;
	float HDOP;      // Hosizontal dilution of precision
	unsigned char checksum;
} gpsState_t;

extern int LN_FD_GPS;
extern struct termios LN_ORIG_TOPTIONS;

int  lnConnect (const char* dev, int baud);
int  lnDisconnect();
int  lnReadMsg (char* dst, int size);
int  lnSendMsg (const char* src, int size);
int  lnParseMsg(gpsState_t* hndlr, char* msg);
void lnPrintState(gpsState_t* state);

#ifdef __cplusplus
}
#endif

#endif
