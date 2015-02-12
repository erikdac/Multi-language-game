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
    "fmt"
    "net"
    "os"
    "strings"
    "strconv"
    "bufio"
)

// The connection settings.
const (
    CONNECTION_HOST = "localhost"
    CONNECTION_PORT = "1337"
    CONNECTION_TYPE = "tcp"
)

// A hashmap holding all the clients.
var clientList map[net.Conn]Client

// Simple banner to be sent when clients connects.
var BANNER = "Welcome to server!"

// The struct for clients. 
// Holds the output channel which all data sent to the client will pass and 
// the clients connection.
type Client struct {
    output chan []byte
    connection net.Conn
}

func main() {

    // Initialize clientList
    clientList = make(map[net.Conn]Client)

    // Sets up the server.
    socket, err := net.Listen(CONNECTION_TYPE, ":" + CONNECTION_PORT)
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
 * Creates the client with its connection and channel. Also adds it to  
 * the clientList with its remote adress as a "key".
 * Returns a pointer to the client. 
 */
func createClient(connection net.Conn) Client {
    client := new(Client)
    client.connection = connection
    client.output = make(chan []byte)

    // Adds to clientList
    clientList[connection] = *client

    return *client
}

/**
 * This function is called everytime a new client has connected. 
 * It sets up writer and reader functions so that the users can 
 * talk to the servers.
 */
func handleRequest(client Client) {
    go client.writer()
    go client.reader()
    client.output <- []byte(BANNER)
}

/**
 * The protocol for streaming out all the data. It starts by reading the 
 * input that the server wants to send out to the client, which includes 
 * the opcode at the beginning. If the client isn't connected anymore it
 * will break its loop. 
 * 
 * THE PROTOCOL:
 * 1. Takes the data as an byte array.
 * 2. Sends the data.
 * 3. Ends by sending a null-byte.
 *
 * The message sent will look something like:
 * ['H', 'e', 'l', 'l', 'o', \0]
 *
 */
func (client *Client) writer() {
    for {
        data, isConnected := <- client.output
        if !isConnected {
            break
        }
        data = append(data, 0)
        client.connection.Write(data) 
    }
}

/**
 * This method will read the data from the client and returns it 
 * as a byte-array. It will then read until it reaches a null-byte and 
 * call the function 'handleInput' with the data excluding the null-byte.
 */
func (client *Client) reader() {
    input := make([]byte, 256)
    data := []byte{}
    for {
        n, err := client.connection.Read(input)
        if err != nil {
            client.disconnect()
            break
        }

        for i := 0; i < n; i++ {
            if input[i] == 0 && len(data) > 0 {
                handleInput(client, []byte(data))
                data = data[:0] // Empty slice
            } else if input[i] != 0 {
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
            c.output <- data
        }
    }
}

/**
 * This method is called when a client has diconnected. It closes 
 * the connection with the client and removes it from the clientList.
 */
func (client *Client) disconnect() {
    close(client.output)
    delete(clientList, client.connection)
    client.connection.Close()
}

/**
 * The menu that the server-user sees. It is used to manage the server during runtime.
 * It uses the readKeyboard()-method to read from the keyboard and converts the choice 
 * to an integer.
 */
func serverMenu() {
    for {
        fmt.Println()
        fmt.Println("(1) Online clients,(2) Exit")
        choice, _ := strconv.Atoi(readKeyboard())

        switch choice {
            case 1:
                onlineList()
                break
            case 2:
                fmt.Println("SHUTTING DOWN!")
                os.Exit(0)
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
    fmt.Println("Online clients: ")
    for _, c := range clientList {
        fmt.Println(c.connection.RemoteAddr())
    }
}
