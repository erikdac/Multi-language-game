package gamestruct

import (
	"net"
	"encoding/json"
	"errors"
	"../nethandler"
)

type Client struct {
	net			nethandler.Nethandler
	Player     	Player
	Input		(chan map[string]string)
}

func NewClient(connection net.Conn) *Client {
	client := new(Client)
	client.net = nethandler.New(connection)
	client.Input = make(chan map[string]string, 16)
	return client
}

func (client *Client) Login() bool {
	for {
		input, err := client.net.ReadPacket()
		if err != nil {
			return false
		}

		data, errorResponse := parseJson(input)
		if errorResponse != nil {
			client.net.Write(errorResponse)
			continue
		} 

		player, err := checkLogin(data)
		if err != nil {
			data,  _ := LoginPacket(false, Player{})
			client.net.Write(data)
		} else {
			client.Player = player
		    data, _ := LoginPacket(true, player)
			client.net.Write(data)
			return true;
		}
	}
	return false
}

func (client *Client) sendPacket(data []byte) {
	client.net.Write(data);
}

func (client * Client) Reader() (error) {
	for {
		data, err := client.net.ReadPacket()
		if err != nil {
			if _, ok := NameToClient[client.Player.Name]; ok {
				return err
			}
			break
		}

		json, errorResponse := parseJson(data)
		if errorResponse != nil {
			client.net.Write(errorResponse)
		} else {
			client.Input <- json
		}
	}
	return nil;
}

// Function used to handle whatever data the server has recieved from the user.
func (client *Client) HandleInput(data map[string]string) (error) {
	if data["Type"] == "Movement" {
		client.Player.movement(data)
	} else if data["Type"] == "Attack" {
		if data["Condition"] == "Start" {
			client.Player.target = data["Victim"]
		} else if data["Condition"] == "Stop" {
			client.Player.target = ""
		}
	} else {
		return errors.New("Failed Package")
	}
	return nil
}

func parseJson(input []byte) (map[string]string, []byte) {
	var data map[string]string
	err := json.Unmarshal(input, &data)
	if err != nil {
		error := map[string]string {"Type:": "Error"}
		errorResponse,  _ := json.Marshal(error)
		return nil, errorResponse
	}

	return data, nil
}

func (client *Client) Info() (string) {
	return client.net.Ip() + "\t\t" + client.Player.Name
}

func (client *Client) Disconnect() {
	client.net.Disconnect()
	if &client.Player != nil {
		RemovePlayer(&client.Player)
		logOut(client.Player)
	}
}

func (client *Client) Kick() {
	message := map[string]string {"Type": "Disconnect"}
	data,  _ := json.Marshal(message)
	client.sendPacket(data)
	client.Disconnect()
}
