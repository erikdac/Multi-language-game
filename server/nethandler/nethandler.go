package nethandler

import (
	"net"
	"bufio"
	"encoding/json"
)

type Nethandler struct {
	connection net.Conn
}

func NewNethandler(connection net.Conn) (Nethandler) {
	var handler Nethandler
	handler.connection = connection
	return handler
}

// This method reads the data from the client until it reaches a null-byte
// in which case it will return all the data.
func (handler *Nethandler) ReadPacket() ([]byte, error) {
	socket_reader := bufio.NewReader(handler.connection)
	data, err := socket_reader.ReadBytes('\n')
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
 * 2. Appends a newline-byte to the end of the array.
 * 3. Sends the data.
 *
 * The message sent will look something like:
 * ['H', 'e', 'l', 'l', 'o', \n]
 *
 */
func (handler *Nethandler) Write(data []byte) {
	data = append(data, '\n')
	handler.connection.Write(data)
}

func (handler *Nethandler) Disconnect() {
	handler.connection.Close();
}

func (handler *Nethandler) Ip() (string) {
	return handler.connection.RemoteAddr().String()
}

func ParseJson(input []byte) (map[string]string, []byte) {
	var data map[string]string
	err := json.Unmarshal(input, &data)
	if err != nil {
		error := map[string]string {"Type:": "Error"}
		errorResponse,  _ := json.Marshal(error)
		return nil, errorResponse
	}

	return data, nil
}
