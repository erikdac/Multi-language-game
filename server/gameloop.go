package main

import (
	"time"
	"fmt"
)

const (
	MAX_TICK_RATE = 100
)

var adminCommand = make(chan int)
var newClients = make(chan *Client, 8)
var disconnects = make(chan *Client, 8)

func gameLoop() {
	for {
		processAdmin()
		processNewClients()
		processInput()
		processDisconnects()
	    time.Sleep((1000 / MAX_TICK_RATE) * time.Millisecond)
	}
}

func processAdmin() {
	select {
	case choice := <- adminCommand:
		switch choice {
		case 1:
			onlineList()
			break
		case 2:
			kickPlayer()
			break
		case 3:
			shutdown()
			break
		default:
			fmt.Println("Invalid input!")
		}
		printMenu()
	default:
	}
}

func processNewClients() {
	lim := len(newClients)
	for i := 0; i < lim; i++ {
		client := <- newClients
		addClient(client)
	}
}

func addClient(client *Client) {
	playerToClient[client.player.Name] = client
	AddPlayer(&client.player)
	client.player.sendLocalMap()
	go client.reader()
}

func processInput() {
	for _, c := range playerToClient {
		lim := len(c.input)
		for i := 0; i < lim; i++ {
			data := <- c.input
			c.handleInput(data)
		}
	}
}

// Function used to handle whatever data the server has recieved from the user.
func (client *Client) handleInput(data map[string]string) {
	if data["Type"] == "Movement" {
		client.player.Movement(data)
	} else if data["Type"] == "Attack" {
		client.player.target <- data
	} else {
		fmt.Println("FAILED PACKAGE: ", data)
	}
}

func processDisconnects() {
	lim := len(disconnects)
	for i := 0; i < lim; i++ {
		client := <- disconnects
		client.disconnect()
	}
}
