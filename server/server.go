package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
)

// The connection settings.
const (
	CONNECTION_HOST = "localhost"
	CONNECTION_PORT = "1337"
	CONNECTION_TYPE = "tcp"
)

func main() {

	err := InitiateGame()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	// Sets up the server.
	socket, err := net.Listen(CONNECTION_TYPE, ":"+CONNECTION_PORT)
	if err != nil {
		fmt.Println("Error setting up the server!")
		os.Exit(1)
	}

	// Close the listener when the application closes.
	defer socket.Close()

	fmt.Println("Listening on " + CONNECTION_HOST + ":" + CONNECTION_PORT)

	go serverMenu()
	go gameLoop()

	// Listens for the incoming connections.
	for {
		connection, err := socket.Accept()
		if err != nil {
			fmt.Println("Error accepting: ", err.Error())
			os.Exit(1)
		}

		client := createClient(connection)
		go client.handleRequest()
	}
}

/**
 * The menu that the server-user sees. It is used to manage the server during runtime.
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
 * playerToClient map and prints out the connections remote-adresses.
 */
func onlineList() {
	fmt.Println("IP-address: ", "\t\t", "Player name:")
	for _, c := range playerToClient {
		fmt.Println(c.net.Ip(), "\t\t", c.player.Name)
	}
}

func kickPlayer() {
	fmt.Print("Player name: ")
	name := readKeyboard()
	client, exists := playerToClient[name]
	if exists == true {
		client.kick()
		fmt.Println(name, " has been successfully kicked from server.")
	} else {
		fmt.Println("Player does not exists.")
	}
}

func shutdown() {
	fmt.Println("SHUTTING DOWN!")
	for _, c := range playerToClient {
		c.kick()
	}
	os.Exit(0)
}
