package main

import (
	"net"
	"sync"
	"encoding/json"
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

		data, errorResponse := parseJson(input)
		if errorResponse != nil {
			client.write(errorResponse)
		} else {
			player, err := checkLogin(data)
			loginSuccess := map[string]string {"Type:": "LoginSuccess"}
			if err != nil {
				loginSuccess["Success"] = "no"
				data,  _ := json.Marshal(loginSuccess)
				client.write(data)
			} else {
				client.player = player
				loginSuccess["Success"] = "yes"
				data,  _ := json.Marshal(loginSuccess)
				client.write(data)
				playerJson := map[string]string {"Type:": "Player", "x": string(player.x), "y": string(player.y), "z": string(player.z)}
				data,  _ = json.Marshal(playerJson)
				client.write(data)
				return true
			}
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
				go client.handleInput(data)
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
func (client *Client) handleInput(input []byte) {

	data, errorResponse := parseJson(input)
	if errorResponse != nil {
		client.write(errorResponse)
		return;
	}


	if data["Type"] == "Logout" {
		client.disconnect()
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
	delete(clientList, client.player.name)
	client.connection.Close()
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
