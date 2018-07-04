# gmbs go serial
Only for Windows. Linux version is coming (soon?)
# example
```go
package main

import (
	"log"

	serial "github.com/g0mb4/gg_serial"
)

func main() {
	var err error
	var b byte

	s := serial.NewSerial("COM1", 9600, true, 100)
	defer s.Close()

	err = s.Open()
	if err != nil {
		log.Panicf("%v", err)
	}

	err = s.WriteText("Send me a char!")
	if err != nil {
		log.Printf("%v", err)
	}

	b, err = s.ReadByte()
	if err != nil {
		log.Printf("%v", err)
	} else {
		log.Printf("Read: %c (%d)\n", b, b)
	} 

}
```
