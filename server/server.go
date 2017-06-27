package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"./gamestruct"
)

// The connection settings.
const (
	CONNECTION_PORT = "1338"
	CONNECTION_TYPE = "tcp"
)

func main() {
	err := gamestruct.InitiateGame()
	if err != nil {
		panic(err)
	}

	go authenticationServer()
	go gameServer()
	go gameLoop()

	serverMenu()
}

/**
 * The menu that the server-user see. It is used to manage the server during runtime.
 * It uses the readKeyboard()-method to read from the keyboard and converts the choice
 * to an integer.
 */
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

/**
 * Method used to read from the keyboard. It will remove all unnecessary whitespace
 * and make sure that the input isn't empty. The input will be returned as a string.
 */
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

/**
 * Method for getting a list of all online clients. It goes through the
 * gamestruct.NameToClient map and prints out the connections remote-adresses.
 */
func onlineList() {
	fmt.Println("IP-address:\t\tPlayer name:")
	for _, c := range gamestruct.NameToClient {
		fmt.Println(c.Info())
	}
}

func kickPlayer() {
	fmt.Print("Player name: ")
	name := readKeyboard()
	client, exists := gamestruct.NameToClient[name]
	if exists == true {
		client.Kick()
		fmt.Println(name, " has been successfully kicked from server.")
	} else {
		fmt.Println("Player does not exists.")
	}
}

func shutdown() {
	fmt.Println("SHUTTING DOWN!")
	for _, c := range gamestruct.NameToClient {
		c.Kick()
	}
	os.Exit(0)
}
