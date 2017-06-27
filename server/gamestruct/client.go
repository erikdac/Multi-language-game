package gamestruct

import (
	"net"
	"encoding/json"
	"errors"
	"../nethandler"
	"../nethandler/packets"
	"../dbhandler"
	"./entity"
)

type Client struct {
	net			nethandler.Nethandler
	Player     	entity.Player
	Input		(chan map[string]string)
}

func NewClient(connection net.Conn) *Client {
	client := new(Client)
	client.net = nethandler.New(connection)
	client.Input = make(chan map[string]string, 16)
	return client
}

func (client *Client) Login() bool {
	input, err := client.net.ReadPacket()
	if err != nil {
		return false
	}

	data, errorResponse := nethandler.ParseJson(input)
	if errorResponse != nil {
		client.net.Write(errorResponse)
		return false
	}

	player, err := dbhandler.CheckLogin(data)
	if err != nil {
		data,  _ := packets.Self(entity.Player{})
		client.sendPacket(data)
	} else {
		client.Player = player
	    data, _ := packets.Self(player)
		client.sendPacket(data)
		return true;
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

		json, errorResponse := nethandler.ParseJson(data)
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
		movement(&client.Player, data)
	} else if data["Type"] == "Attack" {
		if data["Condition"] == "Start" {
			client.Player.SetTarget(data["Victim"])
		} else if data["Condition"] == "Stop" {
			client.Player.SetTarget(data[""])
		}
	} else {
		return errors.New("Failed Package")
	}
	return nil
}

func (client *Client) Info() (string) {
	return client.net.Ip() + "\t\t" + client.Player.Name
}

func (client *Client) Disconnect() {
	client.net.Disconnect()
	if &client.Player != nil {
		RemovePlayer(&client.Player)
		dbhandler.LogOut(client.Player)
	}
}

func (client *Client) Kick() {
	message := map[string]string {"Type": "Disconnect"}
	data,  _ := json.Marshal(message)
	client.sendPacket(data)
	client.Disconnect()
}
