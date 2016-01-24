package main

import (
	"net"
	"sync"
	"encoding/json"
	"bufio"
)

// Binds the player names to their clients.
var clientList map[string]*Client

type Client struct {
	connection 		net.Conn
	output_mutex 	sync.Mutex // TODO: Change to a unbuffered channel instead.
	player     		Player
}

/**
 * Creates the client with its connection and channel.
 * Returns a pointer to the client.
 */
func createClient(connection net.Conn) *Client {
	client := new(Client)
	client.connection = connection
	return client
}

/**
 * This function is called everytime a new client has connected.
 * It calls the function 'accountManager()' so that the client can login.
 * If the login is successful it will set up a reader-function so the client
 * can talk to the server. It also adds it to the clientList with its remote
 * adress as a "key".
 */
func (client *Client) handleRequest() {
	if client.login() == true {
		clientList[client.player.Name] = client
		AddPlayer(&client.player)
		go client.reader()
	} else {
		client.connection.Close()
	}
}

func (client *Client) login() bool {
	for {
		input, err := client.readPacket()
		if err != nil {
			return false
		}

		data, errorResponse := parseJson(input)
		if errorResponse != nil {
			client.write(errorResponse)
		} else {
			player, err := checkLogin(data)
			packet := &login_packet {Type: "Login_Success"}
			if err != nil {
				packet.Success = false
				data,  _ := json.Marshal(packet)
				client.write(data)
			} else {
				client.player = player
				packet.Success = true
				packet.Player = player
			    data, _ := json.Marshal(packet)
				client.write(data)
				return true
			}
		}
	}
	return false
}

// This method reads the data from the client until it reaches a null-byte 
// in which case it will return all the data.
func (client *Client) readPacket() ([]byte, error) {
	socket_reader := bufio.NewReader(client.connection)
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
func (client *Client) write(data []byte) {
	data = append(data, 0)
	client.output_mutex.Lock()
	client.connection.Write(data)
	client.output_mutex.Unlock()
}

// This method will read the data from the client and call the method
// handleInput() to deal with whatever kind of input it recieves.
func (client *Client) reader() {
	for {
		data, err := client.readPacket()
		if err != nil {
			client.disconnect()
			break
		}

		go client.handleInput(data)
	}
}

/**
 * Function used to handle whatever data the server has recieved from the user.
 */
func (client *Client) handleInput(input []byte) {

	data, errorResponse := parseJson(input)
	if errorResponse != nil {
		client.write(errorResponse)
		return;
	}

	if data["Type"] == "Movement" {
		Movement(&client.player, data)
	} else {
		for _, c := range clientList {
			if c.connection != client.connection {
				c.write(input)
			}
		}
	}
}

// This method is called when a client has diconnected. It closes the connection 
// with the client, removes it from the Hashmaps and also change its 
// online status in the database to 'false'.
func (client *Client) disconnect() {
	delete(clientList, client.player.Name)
	client.connection.Close()
	RemovePlayer(&client.player)
	client.player.logOut()
}

func parseJson(input []byte) (map[string]string, []byte) {
	var data map[string]string
	err := json.Unmarshal(input, &data)
	if err != nil {
		error := map[string]string {"Type:": "Error"}
		errorResponse,  _ := json.Marshal(error)
		return nil, errorResponse
	} else {
		return data, nil
	}
}
