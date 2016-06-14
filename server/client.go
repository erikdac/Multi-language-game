package main

import (
	"net"
	"encoding/json"
	"fmt"
	"./nethandler"
)

// Binds the player names to their clients.
var clientList map[string]*Client

type Client struct {
	net			nethandler.Nethandler
	player     	Player
}

/**
 * Creates the client with its netection and channel.
 * Returns a pointer to the client.
 */
func createClient(netection net.Conn) *Client {
	client := new(Client)
	client.net = nethandler.New(netection)
	return client
}

/**
 * This function is called everytime a new client has netected.
 * It calls the function 'accountManager()' so that the client can login.
 * If the login is successful it will set up a reader-function so the client
 * can talk to the server. It also adds it to the clientList with its remote
 * adress as a "key".
 */
func (client *Client) handleRequest() {
	if client.login() == true {
		clientList[client.player.Name] = client
		AddPlayer(&client.player)
		client.player.sendLocalMap()
		go client.reader()
	} else {
		client.net.Disconnect()
	}
}

func (client *Client) login() bool {
	for {
		input, err := client.net.ReadPacket()
		if err != nil {
			return false
		}

		data, errorResponse := parseJson(input)
		if errorResponse != nil {
			client.net.Write(errorResponse)
		} else {
			player, err := checkLogin(data)
			packet := &login_packet {Type: "Login_Success"}
			if err != nil {
				packet.Success = false
				data,  _ := json.Marshal(packet)
				client.net.Write(data)
			} else {
				client.player = player
				packet.Success = true
				packet.Player = player
			    data, _ := json.Marshal(packet)
				client.net.Write(data)

				return client.waitForClient()
			}
		}
	}
	return false
}

func (client *Client) waitForClient() (bool) {
	data, err := client.net.ReadPacket()
	if err != nil {
		return false
	}
	ready, fail := parseJson(data)
	if fail != nil {
		return false
	}
	return ready["Type"] == "Ready"
}

func (client *Client) sendPacket(data []byte) {
	client.net.Write(data);
}

// This method will read the data from the client and call the method
// handleInput() to deal with whatever kind of input it recieves.
func (client *Client) reader() {
	for {
		data, err := client.net.ReadPacket()
		if err != nil {
			if _, ok := clientList[client.player.Name]; ok {
				client.disconnect()
			}
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
		client.net.Write(errorResponse)
		return;
	}

	if data["Type"] == "Movement" {
		client.player.Movement(data)
	} else if data["Type"] == "Attack" {
		client.player.target <- data
	} else {
		fmt.Println("FAILED PACKAGE: ", data)
	}
}

// This method is called when a client has dinetected. It closes the netection 
// with the client, removes it from the Hashmaps and also change its 
// online status in the database to 'false'.
func (client *Client) disconnect() {
	delete(clientList, client.player.Name)
	client.net.Disconnect()
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
