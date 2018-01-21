#ifndef __SYS_WIN_H__
#define __SYS_WIN_H__

#include <windows.h>
#include <string.h>
#include "type.h"

void win_open(serial_t *serial, const char* port);
void win_close(serial_t *serial);

void win_write_byte(serial_t *serial, char byte);
char win_read_byte(serial_t *serial);
char win_read_byte_non_blocking(serial_t *serial);

void win_write_string(serial_t *serial, const char* msg);

#endif
