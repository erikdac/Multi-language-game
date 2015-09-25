package main

import (

	"net"
	"fmt"
	"sync"
//	"errors"
	"encoding/json"
	"./network"
)

// Binds the player names to their clients.
var clientList map[string]*Client

type Client struct {
	connection net.Conn
	player     Player
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
		go client.reader()
		clientList[client.player.name] = client
	} else {
		client.connection.Close()
	}
}

func (client *Client) login() bool {

	for {
		input, err := client.readSingleInput()
		if err != nil {
			return false
		}

		var request network.LoginRequest
		err = json.Unmarshal(input, &request)
		if err != nil {

			// For testing
			answer, _ := json.Marshal(map[string]int{"apple": 5, "lettuce": 7})
			fmt.Println(answer)

			client.write([]byte("Incorrect packaging!"))
			return false
		}

		player, err := checkLogin(request)
		if err != nil {
			message := network.LoginSuccess{network.Type{"login"}, "no"}
			data,  _ := json.Marshal(&message)
			client.write(data)
		} else {
			client.player = player
			message := network.LoginSuccess{network.Type{"login"}, "yes"}
			data,  _ := json.Marshal(&message)
			client.write(data)
			return true
		}
	}
	return false
}

// This method reads the data from the client and iterates through it until
// it reaches a null-byte in which case it will return all the data.
//
// If all the data which had been read didnt include a null-byte it will
// append it to a byte-array read another round of data and repeat this
// procedure until it has finally found a null-byte.
func (client *Client) readSingleInput() ([]byte, error) {
	input := make([]byte, 64)
	data := []byte{}
	for {
		n, err := client.connection.Read(input)
		if err != nil {
			return []byte{}, err
		}

		for i := 0; i < n; i++ {
			if input[i] == 0 {
				data = append(data, input[:i]...)
				return data, nil
			}
		}
		data = append(data, input...)
	}
}

// TODO: Only lock write() for each client, use channels instead?
var output_mutex = &sync.Mutex{}

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
	output_mutex.Lock()
	client.connection.Write(data)
	output_mutex.Unlock()
}

// TODO: @param error, 	should use a switch-case for different errors
func (client *Client) writeError() {
	message := network.Error{network.Type{"error"}, "test"}
	data,  _ := json.Marshal(&message)
	client.write(data)
}

/**
 * This method will read the data from the client and return it
 * as a byte-array. It will then read until it reaches a null-byte and
 * call the function 'handleInput' with the data excluding the null-byte.
 */
func (client *Client) reader() {
	input := make([]byte, 128)
	data := []byte{}
	for {
		n, err := client.connection.Read(input)
		if err != nil {
			client.disconnect()
			break
		}

		for i := 0; i < n; i++ {
			if input[i] == 0 {
				go handleInput(client, []byte(data))
				data = data[:0] // Empty slice
			} else {
				data = append(data, input[i])
			}
		}
	}
}

/**
 * Function used to handle whatever data the server has recieved from the user.
 */
func handleInput(client *Client, data []byte) {
	for _, c := range clientList {
		if c.connection != client.connection {
			c.write(data)
		}
	}
}

// This method is called when a client has diconnected. It closes the connection 
// with the client, removes it from the Hashmaps and also change its 
// online status in the database to 'false'.
func (client *Client) disconnect() {
	delete(clientList, client.player.name)
	client.connection.Close()
	client.player.logOut()
}