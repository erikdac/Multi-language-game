package entity

type Player struct {
	Actor

	Level		int
	Health 		int
	Mana		int
}

func NewPlayer(name string, x int, y int) (Player) {
	var player Player
	player.Actor = NewActor(name, x, y)
	player.Level = 1
	player.Health = 88
	player.Mana = 12
	return player
}

func (player Player) MaxHealth() (int) {
	return 100  + (player.Level - 1) * 5
}
