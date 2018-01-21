# gmbs go serial

# example
```go
package main

import (
	"log"

	serial "github.com/g0mb4/gg_serial"
)

func main() {
	var err error
	
	s := serial.NewSerial("COM2", 9600)
	defer s.Close()

	err = s.Open()
	if err != nil {
		log.Printf("%v", err)
	}

	err = s.WriteByte('a')

	if err != nil {
		log.Printf("%v", err)
	}

}
```
