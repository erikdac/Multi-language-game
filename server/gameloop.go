package main

import (
	"time"
	"fmt"
	"errors"
	"./nethandler"
	"./gamestruct"
)

const (
	MAX_TICK_RATE = 100
)

var adminCommand = make(chan int)
var newClients = make(chan *nethandler.Client, 8)
var disconnects = make(chan *nethandler.Client, 8)

func gameLoop(state *gamestruct.GameState) {
	nameToClient := make(map[string]*nethandler.Client) // Binds the player names to their clients.
	runtime := int64((time.Second / time.Nanosecond) / MAX_TICK_RATE)
	for {
		start := time.Now()
		processAdmin(state, nameToClient)
		processClients(state, nameToClient)
		processAi(state)
		processOutput(state, nameToClient)
		elapsed := time.Since(start).Nanoseconds()
		delay := time.Duration(runtime - elapsed)
	    time.Sleep(delay)
	}
}

func processAdmin(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	select {
	case choice := <- adminCommand:
		switch choice {
		case 1:
			onlineList(state, nameToClient)
			break
		case 2:
			kickPlayer(state, nameToClient)
			break
		case 3:
			shutdown(state, nameToClient)
			break
		default:
			fmt.Println("Invalid input!")
		}
		printMenu()
	default:
	}
}

func processClients(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	processNewClients(state, nameToClient)
	processInput(state, nameToClient)
	processPlayers(state, nameToClient)
	processDisconnects(state, nameToClient)
}

func processAi(state *gamestruct.GameState) {
	for _, creature := range state.CreatureList {
		gamestruct.ProcessCreature(state, creature)
	}
}

func processNewClients(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	lim := len(newClients)
	for i := 0; i < lim; i++ {
		client := <- newClients
		gamestruct.AddClient(state, &client.Player)
		nameToClient[client.Player.Name] = client
		go func() {
			err := client.Reader()
			if err != nil {
				disconnects <- client
			}

		}();
	}
}

func processInput(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	for _, c := range nameToClient {
		lim := len(c.Input)
		for i := 0; i < lim; i++ {
			data := <- c.Input
			err := handleClientInput(state, c, data)
			if err != nil {
				fmt.Println("FAILED PACKAGE: ", data)
			}
		}
	}
}

func processPlayers(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	var deadPlayers []*nethandler.Client
	for _, c := range nameToClient {
		gamestruct.ProcessPlayer(state, &c.Player)
		if c.Player.Health <= 0 {
			deadPlayers = append(deadPlayers, c)
		}
	}

	for _, c := range deadPlayers {
		gamestruct.DeadPlayer(state, &c.Player)
		disconnectClient(state, nameToClient, c)
	}
}

func processDisconnects(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	lim := len(disconnects)
	for i := 0; i < lim; i++ {
		client := <- disconnects
		disconnectClient(state, nameToClient, client)
	}
}

func processOutput(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	for name, list := range state.GetAndClearNetPackets() {
		for _, data := range list {
			nameToClient[name].SendPacket(data)
		}
	}
}

// Function used to handle whatever data the server has recieved from the user.
func handleClientInput(state *gamestruct.GameState, client *nethandler.Client, data map[string]string) (error) {
	if data["Type"] == "Movement" {
		gamestruct.Movement(state, &client.Player, data)
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
