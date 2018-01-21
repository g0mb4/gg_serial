#include "sys_linux.h"

void linux_open(serial_t *serial, const char* port){
	serial->port_handle = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (serial->port_handle < 0){
		serial->error = 51;
		return;
	}

	struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr(serial->port_handle, &tty) != 0){
		serial->error = 52;
		linux_close(serial);
		return;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); 	// shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);	// ignore modem controls,
                                    	// enable reading
    tty.c_cflag &= ~(PARENB | PARODD);  // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0){
		serial->error = 53;
		linux_close(serial);
    }
}

void linux_close(serial_t *serial){
	close(serial->port_handle);
}

void linux_write_byte(serial_t *serial, char byte){
	/*TODO*/
}

char linux_read_byte(serial_t *serial){
	/*TODO*/
}

char win_read_byte_non_blocking(serial_t *serial){
	/*TODO*/
}

void linux_write_string(serial_t *serial, const char* msg){
	/*TODO*/
}
