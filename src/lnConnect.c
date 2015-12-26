#include "libNEMA.h"
#include <stdio.h>    // Standard input/output definitions 
#include <unistd.h>   // UNIX standard function definitions 
#include <fcntl.h>    // File control definitions 
#include <errno.h>    // Error number definitions 
#include <termios.h>  // POSIX terminal control definitions 
#include <string.h>   // String function definitions 
#include <sys/ioctl.h>

int FD_GPS = 0;

int lnConnect(const char* dev, int baud){
	struct termios toptions;

	if(!(FD_GPS = open(dev, O_RDWR))){
        	perror("serialport_init: Unable to open port ");
        	return -1;
	}
	
	if (tcgetattr(FD_GPS, &toptions) < 0) {
		perror("serialport_init: Couldn't get term attributes");
		return -1;
	}

	speed_t brate = baud; // let you override switch below if needed

	cfsetispeed(&toptions, brate);
	cfsetospeed(&toptions, brate);

	// 8N1
	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;
	// no flow control
	toptions.c_cflag &= ~CRTSCTS;

    //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset
	// turn on READ & ignore ctrl lines
	toptions.c_cflag |= CREAD | CLOCAL;
	// turn off s/w flow ctrl
	toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
	toptions.c_lflag = ICANON; // make raw
	toptions.c_oflag &= ~OPOST; // make raw

	// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
	toptions.c_cc[VMIN]  = 0;
	toptions.c_cc[VTIME] = 0;
	toptions.c_cc[VEOL]   = '\r';
	toptions.c_cc[VEOL2]  = '\n';
	
	tcsetattr(FD_GPS, TCSANOW, &toptions);
	if( tcsetattr(FD_GPS, TCSAFLUSH, &toptions) < 0) {
		perror("init_serialport: Couldn't set term attributes");
		return -1;
	}

	return FD_GPS;
}
