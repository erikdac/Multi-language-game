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
	runtime := int64((time.Second / time.Nanosecond) / MAX_TICK_RATE)
	for {
		start := time.Now()
		processAdmin()
		processClients()
		processAi()
		elapsed := time.Since(start).Nanoseconds()
		delay := time.Duration(runtime - elapsed)
	    time.Sleep(delay)
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

func processClients() {
	processNewClients()
	processInput()
	processAutoAttack()
	processDisconnects()
}

func processAi() {

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
		if data["Condition"] == "Start" {
				client.player.target = data["Victim"]
		} else if data["Condition"] == "Stop" {
			client.player.target = ""
		}
	} else {
		fmt.Println("FAILED PACKAGE: ", data)
	}
}

func processAutoAttack() {
	for _, c := range playerToClient {
		c.player.Auto_attack()
	}
}

func processDisconnects() {
	lim := len(disconnects)
	for i := 0; i < lim; i++ {
		client := <- disconnects
		client.disconnect()
	}
}
