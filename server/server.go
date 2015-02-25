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
)

// The connection settings.
const (
	CONNECTION_HOST = "localhost"
	CONNECTION_PORT = "1337"
	CONNECTION_TYPE = "tcp"
)

// Binds the player names to their clients.
var clientList map[string]*Client

// Simple banner to be sent when clients connects.
var BANNER = "Welcome to server!"

type Client struct {
	connection net.Conn
	player     Player
}

func main() {

	clientList = make(map[string]*Client)
	playerList = make(map[net.Conn]*Player)

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
		go handleRequest(client)
	}
}

/**
 * Creates the client with its connection and channel.
 * Returns a pointer to the client.
 */
func createClient(connection net.Conn) *Client {
	client := new(Client)
	client.connection = connection
	return client
}

/**
 * This function is called everytime a new client has connected.
 * It calls the function 'accountManager()' so that the client can login.
 * If the login is successful it will set up a reader-function so the client
 * can talk to the server. It also adds it to the clientList with its remote
 * adress as a "key".
 */
func handleRequest(client *Client) {
	if client.login() == true {
		go client.reader()
		clientList[client.player.name] = client
		playerList[client.connection] = &client.player
		client.write([]byte(BANNER))
	} else {
		client.connection.Close()
	}
}

/**
 * The protocol for sending out all the data. If the client isn't connected
 * anymore it will break its loop.
 *
 * THE PROTOCOL:
 * 1. Takes the data as an byte array.
 * 2. Appends a null-byte to the end of the array.
 * 3. Sends the data.
 *
 * The message sent will look something like:
 * ['H', 'e', 'l', 'l', 'o', \0]
 *
 */
func (client *Client) write(data []byte) {
	data = append(data, 0)
	client.connection.Write(data)
}

/**
 * This method will read the data from the client and return it
 * as a byte-array. It will then read until it reaches a null-byte and
 * call the function 'handleInput' with the data excluding the null-byte.
 */
func (client *Client) reader() {
	input := make([]byte, 128)
	data := []byte{}
	for {
		n, err := client.connection.Read(input)
		if err != nil {
			client.disconnect()
			break
		}

		for i := 0; i < n; i++ {
			if input[i] == 0 {
				handleInput(client, []byte(data))
				data = data[:0] // Empty slice
			} else {
				data = append(data, input[i])
			}
		}
	}
}

/**
 * Function used to handle whatever data the server has recieved from the user.
 */
func handleInput(client *Client, data []byte) {
	for _, c := range clientList {
		if c.connection != client.connection {
			c.write(data)
		}
	}
}

// This method is called when a client has diconnected. It closes the connection 
// with the client, removes it from the Hashmaps and also change its 
// online status in the database to 'false'.
func (client *Client) disconnect() {
	delete(clientList, client.player.name)
	delete(playerList, client.connection)
	client.connection.Close()
	client.player.logOut()
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
	if exists != false {
		client.write([]byte("Kicked from server!"))
		client.disconnect()
		fmt.Println(name, " has been successfully kicked from server.")
	} else {
		fmt.Println("Player does not exists.")
	}
}

func shutdown() {
	fmt.Println("SHUTTING DOWN!")
	for _, c := range clientList {
		c.write([]byte("Server shutting down!"))
		c.disconnect()
	}
	os.Exit(0)
}
