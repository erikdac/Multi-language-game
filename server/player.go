package main

import (
	"strconv"
)

type Player struct {
	name 	string
	x   	int
	y 		int
}

func (player *Player) movement(movement map[string]string) {
	player.x, _ = strconv.Atoi(movement["ToX"])
	player.y, _ = strconv.Atoi(movement["ToY"])

	// TODO: Inform all nearby players that the movement has happend.
}
