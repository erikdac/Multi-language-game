package main

type Player struct {
	name string
	x    int
	y    int
}

type Movement struct {
	toX int
	toY int
}

func (player *Player) movement(movement Movement) {
	player.x = movement.toX
	player.y = movement.toY

	// TODO: Inform all nearby players that the movement has happend.
}
