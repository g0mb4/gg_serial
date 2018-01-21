package gg_serial

// #cgo LDFLAGS: -L . -lgg_serial
// #include "gg_serial.h"
import "C"
import (
	"fmt"
	"unsafe"
)

type GG_Serial struct {
	Port   string
	Serial C.serial_t
}

func NewSerial(port string, baud int) *GG_Serial {
	var serial C.serial_t
	serial.baud_rate = C.int(baud)

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
		return fmt.Errorf("open() failed: %d\n", s.Serial.error)
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
		return fmt.Errorf("write_byte() failed: %d\n", s.Serial.error)
	}

	return nil
}

func (s *GG_Serial) WriteText(msg string) error {
	message := C.CString(msg)
	defer C.free(unsafe.Pointer(message))

	C.ggs_write_string(&s.Serial, message)
	if s.Serial.error != 0 {
		return fmt.Errorf("ggs_write_string() failed: %d\n", s.Serial.error)
	}

	return nil
}

func (s *GG_Serial) ReadByte() (byte, error) {
	c := C.ggs_read_byte(&s.Serial)
	if s.Serial.error != 0 {
		return 0, fmt.Errorf("read_byte() failed: %d\n", s.Serial.error)
	}

	return byte(c), nil
}
