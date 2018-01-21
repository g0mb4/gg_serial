#include "gg_serial.h"

void ggs_open(serial_t *serial, const char* port){
	#if defined _WIN32 || defined __CYGWIN__
		win_open(serial, port);
	#elif defined __linux__
		linux_open(serial, port);

	#elif defined __APPLE__

	#endif
}

void ggs_close(serial_t *serial){
	#if defined _WIN32 || defined __CYGWIN__
		win_close(serial);
	#elif defined __linux__
		linux_close(serial);

	#elif defined __APPLE__

	#endif
}

void ggs_write_byte(serial_t *serial, char byte){
	#if defined _WIN32 || defined __CYGWIN__
		win_write_byte(serial, byte);
	#elif defined __linux__
		linux_write_byte(serial, byte);

	#elif defined __APPLE__

	#endif
}

char ggs_read_byte(serial_t *serial){
	#if defined _WIN32 || defined __CYGWIN__
		return win_read_byte(serial);
	#elif defined __linux__
		return linux_read_byte(serial);

	#elif defined __APPLE__

	#endif
}

char ggs_read_byte_non_blocking(serial_t *serial){
	#if defined _WIN32 || defined __CYGWIN__
		return win_read_byte_non_blocking(serial);
	#elif defined __linux__
		return linux_read_byte_non_blocking(serial);

	#elif defined __APPLE__

	#endif
}

void ggs_write_string(serial_t *serial, const char* msg){
	#if defined _WIN32 || defined __CYGWIN__
		win_write_string(serial, msg);
	#elif defined __linux__
		linux_write_string(serial, msg);

	#elif defined __APPLE__

	#endif
}
