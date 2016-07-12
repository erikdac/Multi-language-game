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
		processAdminCommand()
		processNewClients()
		processDisconnects()
	    time.Sleep((1000 / MAX_TICK_RATE) * time.Millisecond)
	}
}

func processAdminCommand() {
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

func processDisconnects() {
	lim := len(disconnects)
	for i := 0; i < lim; i++ {
		client := <- disconnects
		client.disconnect()
	}
}
