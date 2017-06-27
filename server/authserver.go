package main

import (
	"net"
	"./nethandler"
	"./nethandler/packets"
	"./dbhandler"
)

const (
	AUTH_PORT = "1337"
	AUTH_TYPE = "tcp"
)

func authenticationServer() {
	// Sets up the server.
	socket, err := net.Listen(AUTH_TYPE, ":" + AUTH_PORT)
	if err != nil {
		panic("Error setting up the authentication server!")
	}
	defer socket.Close()

    for {
        connection, err := socket.Accept()
        if err == nil {
	        go handleAuthentication(connection)
        }
    }
}

func handleAuthentication(connection net.Conn) {
	netHandler := nethandler.New(connection)
	input, err := netHandler.ReadPacket()
	if err != nil {
		return
	}

	data, errorResponse := nethandler.ParseJson(input)
	if errorResponse != nil {
		netHandler.Write(errorResponse)
		return
	}

	token, err := dbhandler.Authenticate(data)
	tokenData, _ := packets.Token("")
	if err == nil {
		tokenData, _ = packets.Token(token)
	}
	netHandler.Write(tokenData)
}
