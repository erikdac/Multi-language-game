package main

import (
	"net"
    "./dbhandler"
    "./nethandler"
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
    client := nethandler.NewClient(connection)
    if client.Login() == true {
        newClients <- client
    } else {
        client.Disconnect()
    }
}

func disconnectClient(state *gamestruct.GameState, nameToClient map[string]*nethandler.Client, client *nethandler.Client) {
    client.Disconnect()
    if &client.Player != nil {
        gamestruct.RemoveClient(state, &client.Player)
        delete(nameToClient, client.Player.Name)
        dbhandler.LogOut(client.Player)
    }
}
