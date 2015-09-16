// A simple go-server which can either be used to learn how simple
// go-networking works or to built further on for your own project.
// This version uses go-routines to handle each client so that many
// clients can be connected on the same time, independently of each other.
// The clients can send bytes of data and when a null-byte it recived
// the data will be printed out as a string.
//
// On the server-side there is also a menu option. The host can either
// choose to see which connections is active or it can shutdown the server.
//
// Written by Erik Dackander.
// Last updated 2015-01-31.

package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"sync"
)

// The connection settings.
const (
	CONNECTION_HOST = "localhost"
	CONNECTION_PORT = "1337"
	CONNECTION_TYPE = "tcp"
)

func main() {
	clientList = make(map[string]*Client)

	err := resetDatabaseOnlineList()
	if err != nil {
		fmt.Println("Error clearing the database onlinelist!")
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
	for {
		fmt.Println()
		fmt.Println("(1) Online clients, (2) Kick player, (3) Exit")
		choice, _ := strconv.Atoi(readKeyboard())

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
	}
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
 * Method for getting a list of all online clients. It goes through the clientList
 * and prints out the connections remote-adresses.
 */
func onlineList() {
	fmt.Println("IP-address: ", "\t\t", "Player name:")
	for _, c := range clientList {
		fmt.Println(c.connection.RemoteAddr(), "\t\t", c.player.name)
	}
}

func kickPlayer() {
	fmt.Print("Player name: ")
	name := readKeyboard()
	client, exists := clientList[name]
	if exists == true {
		client.write([]byte("Kicked from server!"))
		client.disconnect()
		fmt.Println(name, " has been successfully kicked from server.")
	} else {
		fmt.Println("Player does not exists.")
	}
}

func shutdown() {
	fmt.Println("SHUTTING DOWN!")
	var wg sync.WaitGroup
	for _, c := range clientList {
		wg.Add(1)
		go func() {
			c.write([]byte("Server shutting down!"))
			c.disconnect()
			wg.Done()
		}()
	}
	wg.Wait()
	os.Exit(0)
}
