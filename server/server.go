package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"./dbhandler"
	"./nethandler"
	"./gamestruct"
)

func main() {
	err := dbhandler.ResetOnlineList()
	if err != nil {
		panic("Was not able to reset database online list!")
	}	
	state, err := gamestruct.InitiateGame()
	if err != nil {
		panic(err)
	}

	go authenticationServer()
	go gameServer()
	go gameLoop(state)

	serverMenu()
}

func serverMenu() {
	printMenu()
	for {
		choice, err := strconv.Atoi(readKeyboard())
		if err != nil {
			fmt.Println("Invalid format!")
		} else {
			adminCommand <- choice
		}
	}
}

func printMenu() {
	fmt.Println()
	fmt.Println("(1) Online clients, (2) Kick player, (3) Exit")
}

func readKeyboard() string {
	reader := bufio.NewReader(os.Stdin)
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println(err)
		}
		line = strings.TrimRight(line, " \t\r\n")
		if line != "" {
			return line
		}
	}
	return ""
}

func onlineList(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	fmt.Println("IP-address:\t\tPlayer name:")
	for name, c := range nameToClient {
		fmt.Println(c.Info() + "\t\t" + name)
	}
}

func kickPlayer(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	fmt.Print("Player name: ")
	name := readKeyboard()
	client, exists := nameToClient[name]
	if exists == true {
		disconnectClient(state, nameToClient, client)
		fmt.Println(name, " has been successfully kicked from server.")
	} else {
		fmt.Println("Player does not exists.")
	}
}

func shutdown(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client) {
	fmt.Println("SHUTTING DOWN!")
	for _, c := range nameToClient {
		disconnectClient(state, nameToClient, c)
	}
	os.Exit(0)
}
