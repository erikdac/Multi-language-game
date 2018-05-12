package nethandler

import (
	"net"
	"encoding/json"
	"./packets"
	"../dbhandler"
	"../entity"
)

type Client struct {
	net			Nethandler
	Player     	entity.Player
	Input		(chan map[string]string)
}

func NewClient(connection net.Conn) *Client {
	client := new(Client)
	client.net = NewNethandler(connection)
	client.Input = make(chan map[string]string, 16)
	return client
}

func (client *Client) Login() bool {
	input, err := client.net.ReadPacket()
	if err != nil {
		return false
	}

	data, errorResponse := ParseJson(input)
	if errorResponse != nil {
		client.net.Write(errorResponse)
		return false
	}

	player, err := dbhandler.CheckLogin(data)
	if err != nil {
		data,  _ := packets.Self(entity.Player{})
		client.SendPacket(data)
	} else {
		client.Player = player
	    data, _ := packets.Self(player)
		client.SendPacket(data)
		return true;
	}
	return false
}

func (client *Client) SendPacket(data []byte) {
	client.net.Write(data);
}

func (client * Client) Reader() (error) {
	for {
		data, err := client.net.ReadPacket()
		if err != nil {
			return err
/*
			if _, ok := NameToClient[client.Player.Name]; ok {
				return err
			}
			break
*/
		}

		json, errorResponse := ParseJson(data)
		if errorResponse != nil {
			client.net.Write(errorResponse)
		} else {
			client.Input <- json
		}
	}
	return nil;
}

func (client *Client) Info() (string) {
	return client.net.Ip()
}

func (client *Client) Disconnect() {
	message := map[string]string {"Type": "Disconnect"}
	data,  _ := json.Marshal(message)
	client.SendPacket(data)
	client.net.Disconnect()
}
