package main

// Binds all the players to a client.
var playerList map[Player]Client

type Player struct {
	Name string
	X int
	Y int
}
