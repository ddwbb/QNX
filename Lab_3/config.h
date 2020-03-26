#ifndef _CONFIG_
#define _CONFIG_

#include <devctl.h>

#define LENGTH 1024
#define DEVICE_NAME "/dev/RTOS"

typedef struct {
	struct {
		int a;
		int c;
		int m;
		int count;
		int start;
	} lcg;

	char text[LENGTH];
	char output[LENGTH];
} OtpContext;

#define CYPHER_TEXT __DIOTF(_DCMD_ALL, 1, OtpContext)

#endif
