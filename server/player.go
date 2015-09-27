package main

import (
	"strconv"
)

type Player struct {
	name 	string
	x   	int
	y 		int
	z 		int
}

func (player *Player) movement(movement map[string]string) {
	player.x, _ = strconv.Atoi(movement["ToX"])
	player.y, _ = strconv.Atoi(movement["ToY"])
	player.z, _ = strconv.Atoi(movement["ToZ"])

	// TODO: Inform all nearby players that the movement has happend.
}
