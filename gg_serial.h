#ifndef __GG_SERIAL_H__
#define __GG_SERIAL_H__

#include <stdio.h>

#if defined _WIN32 || defined __CYGWIN__
	#include "sys_win.h"

#elif defined __linux__
	#include "sys_linux.h"

#elif defined __APPLE__
	#include "sys_mac.h"

#endif

void ggs_open(serial_t *serial, const char* port);
void ggs_close(serial_t *serial);

void ggs_write_byte(serial_t *serial, char byte);
char ggs_read_byte(serial_t *serial);

void ggs_write_string(serial_t *serial, const char* msg);

#endif
