#ifndef __LIB_NEMA
#define __LIB_NEMA

typedef struct{
	float Lat;
	float Lon;
	float Speed;
} GpsState;

extern int FD_GPS;

int lnConnect(const char* dev, int baud);
int lnReadMsg(char* dst, int size);

#endif
