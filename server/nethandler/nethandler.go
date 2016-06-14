package nethandler

import (
	"net"
	"sync"
	"bufio"
)

type Nethandler struct {
	connection 		net.Conn
	output_mutex 	sync.Mutex
}

func New(connection net.Conn) (Nethandler) {
	var handler Nethandler
	handler.connection = connection
	return handler
}

// This method reads the data from the client until it reaches a null-byte 
// in which case it will return all the data.
func (handler *Nethandler) ReadPacket() ([]byte, error) {
	socket_reader := bufio.NewReader(handler.connection)
	data, err := socket_reader.ReadBytes(0)
	if err == nil {
		data = data[:len(data)-1]
	}
	return data, err
}

/**
 * The protocol for sending out all the data. 
 *
 * THE PROTOCOL:
 * 1. Takes the data as an byte array.
 * 2. Appends a null-byte to the end of the array.
 * 3. Sends the data.
 *
 * The message sent will look something like:
 * ['H', 'e', 'l', 'l', 'o', \0]
 *
 */
func (handler *Nethandler) Write(data []byte) {
	data = append(data, 0)
	handler.output_mutex.Lock()
	handler.connection.Write(data)
	handler.output_mutex.Unlock()
}

func (handler *Nethandler) Disconnect() {
	handler.connection.Close();
}

func (handler *Nethandler) Ip() (string) {
	return handler.connection.RemoteAddr().String()
}
