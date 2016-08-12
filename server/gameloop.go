package main

import (
	"time"
	"fmt"
	"./gamestruct"
)

const (
	MAX_TICK_RATE = 100
)

var adminCommand = make(chan int)
var newClients = make(chan *gamestruct.Client, 8)
var disconnects = make(chan *gamestruct.Client, 8)

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
	for _, creature := range gamestruct.CreatureList {
		creature.Process()
	}
}

func processNewClients() {
	lim := len(newClients)
	for i := 0; i < lim; i++ {
		client := <- newClients
		gamestruct.AddClient(client)
		go func() {
			err := client.Reader()
			if err != nil {
				disconnects <- client
			}

		}();
	}
}

func processInput() {
	for _, c := range gamestruct.PlayerToClient {
		lim := len(c.Input)
		for i := 0; i < lim; i++ {
			data := <- c.Input
			err := c.HandleInput(data)
			if err != nil {
				fmt.Println("FAILED PACKAGE: ", data)
			}
		}
	}
}

func processAutoAttack() {
	for _, c := range gamestruct.PlayerToClient {
		c.Player.Auto_attack()
	}
}

func processDisconnects() {
	lim := len(disconnects)
	for i := 0; i < lim; i++ {
		client := <- disconnects
		client.Disconnect()
	}
}
