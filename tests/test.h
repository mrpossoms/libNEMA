#ifndef __TEST
#define __TEST

#include <stdio.h>
#include <strings.h>
#include <stdarg.h>
#include <assert.h>

void __title(const char* msg){
	printf("\n%s\n", msg);
}

void __msg(const char* msg, ...){
	va_list arg;
	char buf[256];

	va_start(arg, msg);
		vsnprintf(buf, sizeof(buf), msg, arg);
		printf("\t%s\n", buf);
	va_end(arg);
}



#endif
