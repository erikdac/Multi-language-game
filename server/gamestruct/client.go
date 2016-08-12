package gamestruct

import (
	"net"
	"encoding/json"
	"errors"
	"../nethandler"
)

type Client struct {
	Net			nethandler.Nethandler
	Player     	Player
	Input		(chan map[string]string)
}

func NewClient(connection net.Conn) *Client {
	client := new(Client)
	client.Net = nethandler.New(connection)
	client.Input = make(chan map[string]string, 16)
	return client
}

func (client *Client) Login() bool {
	for {
		input, err := client.Net.ReadPacket()
		if err != nil {
			return false
		}

		data, errorResponse := parseJson(input)
		if errorResponse != nil {
			client.Net.Write(errorResponse)
		} else {
			player, err := checkLogin(data)
			packet := &login_packet {Type: "Login_Success"}
			if err != nil {
				packet.Success = false
				data,  _ := json.Marshal(packet)
				client.Net.Write(data)
			} else {
				client.Player = player
				packet.Success = true
				packet.Player = player
			    data, _ := json.Marshal(packet)
				client.Net.Write(data)
				return true;
			}
		}
	}
	return false
}

func (client *Client) sendPacket(data []byte) {
	client.Net.Write(data);
}

func (client *Client) Reader() (error) {
	for {
		data, err := client.Net.ReadPacket()
		if err != nil {
			if _, ok := PlayerToClient[client.Player.Name]; ok {
				return err
			}
			break
		}

		json, errorResponse := parseJson(data)
		if errorResponse != nil {
			client.Net.Write(errorResponse)
		} else {
			client.Input <- json
		}
	}
	return nil;
}

// Function used to handle whatever data the server has recieved from the user.
func (client * Client) HandleInput(data map[string]string) (error) {
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
	} else {
		return data, nil
	}
}

func (client *Client) Disconnect() {
	RemovePlayer(&client.Player)
	delete(PlayerToClient, client.Player.Name)
	client.Net.Disconnect()
	client.Player.logOut()
}

func (client *Client) Kick() {
	message := map[string]string {"Type": "Disconnect"}
	data,  _ := json.Marshal(message)
	client.sendPacket(data)
	client.Disconnect()
}
