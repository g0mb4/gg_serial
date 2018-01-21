#ifndef __TYPE_H__
#define __TYPE_H__

#if defined _WIN32 || defined __CYGWIN__
	#define SHANDLE 	void *

#elif defined __linux__
	#define SHANDLE 	int
	
#endif

typedef struct S_SERIAL {
	SHANDLE port_handle;
	int baud_rate;
	int error;
} serial_t;

#endif
