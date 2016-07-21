package main

import (
	"time"
	"math/rand"
)

type Creature struct {
	Actor

	Type 		string
	spawnX		int
	spawnY		int
	Health		int
	max_health 	int

	target		string
}

func (creature *Creature) Process() {
	if len(creature.target) > 0 {
		creature.combat()
	} else {
		creature.out_of_combat()
	}
}

func (creature *Creature) combat() {

}

func (creature *Creature) move(newX int, newY int) {
	newSectionX, newSectionY := SliceMap(newX, newY)
	oldSectionX, oldSectionY := SliceMap(creature.X, creature.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := creature_map[oldSectionX][oldSectionY]
		delete(oldSection, creature.Name)
		sendCreatureUpdate(creature, true)
		creature.X = newX
		creature.Y = newY
		newSection := creature_map[newSectionX][newSectionY]
		newSection[creature.Name] = creature
	} else {
		creature.X = newX
		creature.Y = newY
	}
	
	sendCreatureUpdate(creature, false)
}

func (creature *Creature) out_of_combat() {
	creature.regnerate()
	creature.walk()
}

func (creature *Creature) regnerate() {
	if creature.Health < creature.max_health  && creature.cooldownMS("REGNERATE") > 1000 {
		hp_reg := int(float64(creature.max_health) * 0.05)
		if hp_reg < 1 {
			hp_reg = 1
		}

		if creature.max_health - creature.Health  < hp_reg {
			hp_reg = creature.max_health - creature.Health
		}
		creature.Health += hp_reg
		creature.Actor.cooldowns["REGNERATE"] = time.Now()
		sendCreatureUpdate(creature, false)
	}
}

func (creature *Creature) walk() {
	if creature.cooldownMS("MOVEMENT") > 5000 {
		switch(rand.Intn(4)) {
		case 0:
			if environment_map[creature.X][creature.Y - 1].isWalkable && creature.distanceTo(creature.spawnX, creature.spawnY + 1) <= 10 {
				creature.move(creature.X, creature.Y - 1)
				break
			}
		case 1:
			if environment_map[creature.X + 1][creature.Y].isWalkable && creature.distanceTo(creature.spawnX - 1, creature.spawnY) <= 10 {
				creature.move(creature.X + 1, creature.Y)
				break
			}
		case 2:
			if environment_map[creature.X][creature.Y + 1].isWalkable && creature.distanceTo(creature.spawnX, creature.spawnY - 1) <= 10 {
				creature.move(creature.X, creature.Y + 1)
				break
			}
		case 3:
			if environment_map[creature.X - 1][creature.Y].isWalkable && creature.distanceTo(creature.spawnX + 1, creature.spawnY) <= 10 {
				creature.move(creature.X - 1, creature.Y)
				break
			}
		default:
		}

		delay := time.Millisecond * time.Duration(rand.Intn(1000))
		creature.Actor.cooldowns["MOVEMENT"] = time.Now().Add(delay)
	}
}
