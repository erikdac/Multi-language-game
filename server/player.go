package main

import (
	"./network"
)

type Player struct {
	name string
	x    int
	y    int
}

func (player *Player) movement(movement network.Movement) {
	player.x = movement.ToX
	player.y = movement.ToY

	// TODO: Inform all nearby players that the movement has happend.
}
