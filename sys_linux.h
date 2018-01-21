#ifndef __SYS_LINUX_H__
#define __SYS_LINUX_H__

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "type.h"

void linux_open(serial_t *serial, const char* port);
void linux_close(serial_t *serial);

void linux_write_byte(serial_t *serial, char byte);
char linux_read_byte(serial_t *serial);
void linux_write_string(serial_t *serial, const char* msg);

#endif
