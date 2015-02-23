package main

import (
	"net"
)

// Binds all the players to a client.
var playerList map[net.Conn]*Player

type Player struct {
	name string
	x    int
	y    int
}
