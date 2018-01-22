#include "sys_win.h"

void win_open(serial_t *serial, const char* port){
	char port_name[128];

	sprintf_s(port_name, 128, "\\\\.\\%s", port);	// generate generic port name

	/* check if port is already open */
	if(serial->port_handle != UNOPENED){
		win_close(serial);
	}

	serial->error = 0;

	/* open handle */
	serial->port_handle = CreateFile(port_name,
					GENERIC_READ | GENERIC_WRITE,
					0,
					0,
					OPEN_EXISTING,
					0,
					0);

	/* check error at opening */
	if(serial->port_handle == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND){
			serial->error = 1;
			win_close(serial);
			return;
		}
		serial->error = 2;
		win_close(serial);
		return;
	}

	/* setting up serialport
	 * src: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363214(v=vs.85).aspx
	 */
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	/* check error at getting default params */
	if (!GetCommState(serial->port_handle, &dcbSerialParams)) {
		serial->error = 3;
		win_close(serial);
		return;
	}

	/* change params */
	dcbSerialParams.BaudRate = (DWORD)serial->baud_rate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	dcbSerialParams.fBinary = TRUE;
	dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
	dcbSerialParams.fOutxCtsFlow = FALSE;
	dcbSerialParams.fOutxDsrFlow = FALSE;
	dcbSerialParams.fDsrSensitivity = FALSE;

	/* check error at setting params */
	if (!SetCommState(serial->port_handle, &dcbSerialParams)) {
		serial->error = 4;
		win_close(serial);
		return;
	}

	/* timeout values for non-blocking mode
	 * src: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363190(v=vs.85).aspx
	 */
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = (DWORD)serial->read_timeout;	 // ms
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;	// ms
	timeouts.WriteTotalTimeoutMultiplier = 10;

	/* chekc error at setting timeout values for non-blocking mode */
	if (!SetCommTimeouts(serial->port_handle, &timeouts)) {
		serial->error = 5;
		win_close(serial);
		return;
	}
}

void win_close(serial_t *serial){
	if (serial->port_handle != UNOPENED) {
		CloseHandle(serial->port_handle);
		serial->port_handle = UNOPENED;
	}
}

void win_write_byte(serial_t *serial, char byte){
	serial->error = 0;

	if (serial->port_handle == UNOPENED) {
		serial->error = 6;
		return;
	}

	char byteWrite = byte;
	DWORD dwBytesWritten = 0;

	/* write to port */
	if (!WriteFile(serial->port_handle, &byteWrite, 1, &dwBytesWritten, NULL)) {
		serial->error = 7;
		return;
	}

	/* no bytes were written */
	if ((int)dwBytesWritten == 0) {
		serial->error = 8;
		return;
	}
}

char win_read_byte(serial_t *serial){
	serial->error = 0;

	if (serial->port_handle == UNOPENED) {
		serial->error = 6;
		return 0;
	}

	char byte_read = 0;
	DWORD dwBytesRead = 0;

	if(serial->blocking){
		for(;;) {
			if (!ReadFile(serial->port_handle, &byte_read, 1, &dwBytesRead, NULL)) {
				serial->error = 9;
				return 0;
			}
			/* no bytes were read so wait longer */
			if ((int)dwBytesRead != 1) {
				continue;
			/* ok, we can proceed */
			} else {
				break;
			}
		}
	} else {
		if (!ReadFile(serial->port_handle, &byte_read, 1, &dwBytesRead, NULL)) {
			serial->error = 9;
			return 0;
		}

		/* no bytes were read so wait longer */
		if ((int)dwBytesRead != 1) {
			serial->error = 10;
			return 0;
		}
	}

	return byte_read;
}

void win_write_string(serial_t *serial, const char* msg){
	serial->error = 0;

	if (serial->port_handle == UNOPENED) {
		serial->error = 6;
		return;
	}

	unsigned int n = strlen(msg);
	DWORD dwBytesWritten = 0;

	/* same as win_write_byte() expect from the size */
	if (!WriteFile(serial->port_handle, msg, n, &dwBytesWritten, NULL)) {
		serial->error = 7;
		return;
	}

	if ((int)dwBytesWritten != (int)n) {
		serial->error = 8;
		return;
	}
}
