#ifndef _CONFIG_
#define _CONFIG_

#include <devctl.h>

#define LENGTH 1024
#define DEVICE_NAME "/dev/RTOS"
#define CTL_CODE_START_HANLDING 1

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

#define CYPHER_TEXT __DIOTF(_DCMD_ALL, CTL_CODE_START_HANLDING, OtpContext)

#endif
