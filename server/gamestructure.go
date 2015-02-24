package main

import (
	"net"
)

// Binds all the clients connections to their players.
var playerList map[net.Conn]*Player

type Player struct {
	name string
	x    int
	y    int
}
