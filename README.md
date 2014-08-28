libNEMA
=======

C implementation of a NEMA 183 GPS message parser, which also handles serial connections to GPS receivers.
This library is designed to be compiled on unix based systems. Aside from dependence on system libraries, this
library is self contained and requires few prerequsites.

Requirements
============

* gcc
* GNU Make

Usage
============

Using libNEMA is very easy. You can start reading and parsing NEMA messages from a serial device in 3 function calls! First you will need to connect to the GPS device in order to obtain a file descriptor like so.

`int fd = lnConnect("/dev/ttyAMA0", 9600);`

Here we connect to a device that is located at the path `/dev/ttyAMA0` and is running at 9600 baud. Now we need to read some data from the device.

```
unsigned char buf[1024];
lnReadMsg(buf, 1024);
```

This reads the available information into a statically allocated buffer. The size of the buffer is also passed to libNEMA to avoid mistakenly modifiying the wrong data. Finally we can parse the message.

```
GpsState state;
lnParseMsg(&state, buf);
```

The `GpsState` type is a struct that contains lat, long, altitude, speed, time, fix and a variety of other useful pieces of information that have been decoded from the messages.
