package gamestruct

import (
	"time"
	"math/rand"
)

const (
	RESPAWN_TIME = 30000
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

func NewCreature(creatureType string, name string, x int, y int, health int) (* Creature) {
	return &Creature{
			Actor{name, 
				x, y,
				map[string]time.Time{},
			}, 
			creatureType, 
			x, y, 
			10, 10,
			"", 
	}
}

func (creature *Creature) Process() {
	if creature.Health <= 0 {
		creature.dead()
	} else if len(creature.target) > 0 {
		creature.combat()
		creature.checkTarget()
	} else {
		creature.outOfCombat()
		creature.findTarget()
	}
}

func (creature *Creature) dead() {
	if creature.cooldownMS("DEAD") > RESPAWN_TIME {
		creature.X = creature.spawnX
		creature.Y = creature.spawnY
		creature.Health = creature.max_health
		x, y := sliceMap(creature.X, creature.Y)
		creatureMap[x][y][creature.Name] = creature
		sendCreatureUpdate(creature)
	}
}

func (creature *Creature) checkTarget() {
	if _, ok := NameToClient[creature.target]; !ok {
		creature.target = ""
		creature.findTarget()
	} else {
		target := NameToClient[creature.target].Player
		if creature.distanceTo(target.X, target.Y) > 5 {
			creature.target = ""
			creature.findTarget()
		}
	}
}

func (creature *Creature) findTarget() {
	shortestDistance := 5
	for _, p := range creature.localPlayerMap() {
		dist := creature.distanceTo(p.X, p.Y)
		if dist <= shortestDistance {
			shortestDistance = dist
			creature.target = p.Name
		}
	}
}

func (creature *Creature) combat() {

}

func (creature *Creature) move(newX int, newY int) {
	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(creature.X, creature.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := creatureMap[oldSectionX][oldSectionY]
		delete(oldSection, creature.Name)
		sendActorRemoved(creature.Actor)
		creature.X = newX
		creature.Y = newY
		newSection := creatureMap[newSectionX][newSectionY]
		newSection[creature.Name] = creature
	} else {
		creature.X = newX
		creature.Y = newY
	}
	
	sendCreatureUpdate(creature)
}

func (creature *Creature) outOfCombat() {
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
		sendCreatureUpdate(creature)
	}
}

func (creature *Creature) walk() {
	if creature.cooldownMS("MOVEMENT") > 5000 {
		switch(rand.Intn(8)) {
		case 0:
			if environmentMap[creature.X][creature.Y - 1].isWalkable && creature.distanceTo(creature.spawnX, creature.spawnY + 1) <= 10 {
				creature.move(creature.X, creature.Y - 1)
				break
			}
		case 1:
			if environmentMap[creature.X + 1][creature.Y - 1].isWalkable && creature.distanceTo(creature.spawnX - 1, creature.spawnY + 1) <= 10 {
				creature.move(creature.X + 1, creature.Y - 1)
				break
			}
		case 2:
			if environmentMap[creature.X + 1][creature.Y].isWalkable && creature.distanceTo(creature.spawnX - 1, creature.spawnY) <= 10 {
				creature.move(creature.X + 1, creature.Y)
				break
			}
		case 3:
			if environmentMap[creature.X + 1][creature.Y + 1].isWalkable && creature.distanceTo(creature.spawnX - 1, creature.spawnY - 1) <= 10 {
				creature.move(creature.X + 1, creature.Y + 1)
				break
			}
		case 4:
			if environmentMap[creature.X][creature.Y + 1].isWalkable && creature.distanceTo(creature.spawnX, creature.spawnY - 1) <= 10 {
				creature.move(creature.X, creature.Y + 1)
				break
			}
		case 5:
			if environmentMap[creature.X - 1][creature.Y - 1].isWalkable && creature.distanceTo(creature.spawnX + 1, creature.spawnY + 1) <= 10 {
				creature.move(creature.X - 1, creature.Y - 1)
				break
			}
		case 6:
			if environmentMap[creature.X - 1][creature.Y].isWalkable && creature.distanceTo(creature.spawnX + 1, creature.spawnY) <= 10 {
				creature.move(creature.X - 1, creature.Y)
				break
			}
		case 7:
			if environmentMap[creature.X - 1][creature.Y + 1].isWalkable && creature.distanceTo(creature.spawnX + 1, creature.spawnY - 1) <= 10 {
				creature.move(creature.X - 1, creature.Y + 1)
				break
			}
		}

		delay := time.Millisecond * time.Duration(rand.Intn(1000))
		creature.Actor.cooldowns["MOVEMENT"] = time.Now().Add(delay)
	}
}

func (victim * Creature) attacked(attacker string, damage int) {
	victim.Health -= damage
	if victim.Health <= 0 {
		x, y := sliceMap(victim.X, victim.Y)
		delete(creatureMap[x][y], victim.Name)
		sendActorRemoved(victim.Actor)
		victim.Actor.cooldowns["DEAD"] = time.Now()
	} else {
		sendCreatureUpdate(victim)
	}
}
