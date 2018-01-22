package gg_serial

// #cgo LDFLAGS: -L . -lgg_serial
// #include "gg_serial.h"
import "C"
import (
	"fmt"
	"unsafe"
)

var error_messages []string = []string{
	"SUCCESS",
	"ERROR_PORT_NOT_FOUND",        // 1	- WIN
	"ERROR_INVALID_HANDLE_VALUE",  // 2
	"ERROR_GET_COMM_STATE",        // 3
	"ERROR_SET_COMM_STATE",        // 4
	"ERROR_SET_COMM_TIMEOUTS",     // 5
	"ERROR_PORT_NOT_OPEN",         // 6
	"ERROR_WRITE_PORT_FAILED",     // 7
	"ERROR_NO_BYTES_WERE_WRITTEN", // 8
	"ERROR_READ_PORT_FAILED",      // 9
	"ERROR_NO_BYTES_WERE_READ"}

type GG_Serial struct {
	Port   string
	Serial C.serial_t
}

func NewSerial(port string, baud int, blocking bool, read_timeout int) *GG_Serial {
	var serial C.serial_t
	serial.baud_rate = C.int(baud)
	serial.blocking = 0
	if blocking {
		serial.blocking = 1
	}
	serial.read_timeout = C.int(read_timeout)

	return &GG_Serial{
		Port:   port,
		Serial: serial,
	}
}

func (s *GG_Serial) Open() error {
	port := C.CString(s.Port)
	defer C.free(unsafe.Pointer(port))

	C.ggs_open(&s.Serial, port)

	if s.Serial.error != 0 {
		return fmt.Errorf("%s", error_messages[s.Serial.error])
	}

	return nil
}

func (s *GG_Serial) Close() {
	C.ggs_close(&s.Serial)
}

func (s *GG_Serial) WriteByte(b byte) error {
	var w C.char = C.char(b)

	C.ggs_write_byte(&s.Serial, w)
	if s.Serial.error != 0 {
		return fmt.Errorf("%s", error_messages[s.Serial.error])
	}

	return nil
}

func (s *GG_Serial) WriteText(msg string) error {
	message := C.CString(msg)
	defer C.free(unsafe.Pointer(message))

	C.ggs_write_string(&s.Serial, message)
	if s.Serial.error != 0 {
		return fmt.Errorf("%s", error_messages[s.Serial.error])
	}

	return nil
}

func (s *GG_Serial) ReadByte() (byte, error) {
	c := C.ggs_read_byte(&s.Serial)
	if s.Serial.error != 0 {
		return 0, fmt.Errorf("%s", error_messages[s.Serial.error])
	}

	return byte(c), nil
}
