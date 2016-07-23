package main

import (
	"net"
	"encoding/json"
	"./nethandler"
)

type Client struct {
	net			nethandler.Nethandler
	player     	Player
	input		(chan map[string]string)
}

func createClient(netection net.Conn) *Client {
	client := new(Client)
	client.net = nethandler.New(netection)
	client.input = make(chan map[string]string, 16)
	return client
}

func (client *Client) handleRequest() {
	if client.login() == true {
		newClients <- client
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
				return true;
			}
		}
	}
	return false
}

func (client *Client) sendPacket(data []byte) {
	client.net.Write(data);
}

func (client *Client) reader() {
	for {
		data, err := client.net.ReadPacket()
		if err != nil {
			if _, ok := playerToClient[client.player.Name]; ok {
				disconnects <- client
			}
			break
		}

		json, errorResponse := parseJson(data)
		if errorResponse != nil {
			client.net.Write(errorResponse)
		} else {
			client.input <- json
		}
	}
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

func (client *Client) disconnect() {
	RemovePlayer(&client.player)
	delete(playerToClient, client.player.Name)
	client.net.Disconnect()
	client.player.logOut()
}

func (client *Client) kick() {
	message := map[string]string {"Type": "Disconnect"}
	data,  _ := json.Marshal(message)
	client.sendPacket(data)
	client.disconnect()
}
