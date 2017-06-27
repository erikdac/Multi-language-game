package main

import (
	"net"
	"./gamestruct"
)

const (
	GAME_PORT = "1338"
	GAME_TYPE = "tcp"
)

func gameServer() {
	// Sets up the server.
	socket, err := net.Listen(GAME_TYPE, ":" + GAME_PORT)
	if err != nil {
		panic("Error setting up the game server!")
	}
	defer socket.Close()

    for {
        connection, err := socket.Accept()
        if err == nil {
            go newConnection(connection);
        }
    }
}

func newConnection(connection net.Conn) {
    client := gamestruct.NewClient(connection)
    if client.Login() == true {
        newClients <- client
    } else {
        client.Disconnect()
    }
}
