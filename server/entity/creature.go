package entity

type Creature struct {
	Actor

	Type 		string
	spawnX		int
	spawnY		int
	Health		int
	maxHealth 	int
}

func NewCreature(creatureType string, name string, x int, y int, health int) (* Creature) {
	var creature Creature
	creature.Actor = NewActor(name, x, y)
	creature.Type = creatureType
	creature.spawnX = x
	creature.spawnY = y
	creature.maxHealth = health
	creature.Health = creature.maxHealth
	return &creature
}

func (creature *Creature) Resurrect() {
	creature.X = creature.spawnX
	creature.Y = creature.spawnY
	creature.Health = creature.maxHealth
}

func (creature Creature) MaxHealth() (int) {
	return creature.maxHealth
}

func (creature Creature) SpawnLocation() (int, int) {
	return creature.spawnX, creature.spawnY
}
