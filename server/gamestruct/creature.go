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
	maxHealth 	int

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
		creature.Health = creature.maxHealth
		creatureMap[creature.X][creature.Y][creature.Name] = creature
		sendCreatureUpdate(*creature)
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
	newSecX, newSecY := sliceMap(newX, newY)
	oldSecX, oldSecY := sliceMap(creature.X, creature.Y)

	if newSecX != oldSecX || newSecY != oldSecY {
		oldSection := creatureMap[oldSecX][oldSecY]
		delete(oldSection, creature.Name)
		if newSecY < oldSecY {
			sendActorRemoved(creature.Actor, NORTH)
		} else if newSecX > oldSecX {
			sendActorRemoved(creature.Actor, EAST)			
		} else if newSecY > oldSecY {
			sendActorRemoved(creature.Actor, SOUTH)
		} else if newSecX < oldSecX {
			sendActorRemoved(creature.Actor, WEST)			
		}
		creature.X = newX
		creature.Y = newY
		newSection := creatureMap[newSecX][newSecY]
		newSection[creature.Name] = creature
	} else {
		creature.X = newX
		creature.Y = newY
	}
	
	sendCreatureUpdate(*creature)
}

func (creature *Creature) outOfCombat() {
	creature.regnerate()
	creature.walk()
}

func (creature *Creature) regnerate() {
	if creature.Health < creature.maxHealth  && creature.cooldownMS("REGNERATE") > 1000 {
		hp_reg := int(float64(creature.maxHealth) * 0.05)
		if hp_reg < 1 {
			hp_reg = 1
		}

		if creature.maxHealth - creature.Health  < hp_reg {
			hp_reg = creature.maxHealth - creature.Health
		}
		creature.Health += hp_reg
		creature.Actor.cooldowns["REGNERATE"] = time.Now()
		sendCreatureUpdate(*creature)
	}
}

func (creature *Creature) walk() {
	if creature.cooldownMS("MOVEMENT") > 5000 {

		// Just renaming for easy reading. 
		x, y := creature.X, creature.Y
		sX, sY := creature.spawnX, creature.spawnY

		switch(rand.Intn(8)) {
		case 0:
			if environmentMap[x][y - 1].isWalkable && creature.distanceTo(sX, sY + 1) <= 10 {
				creature.move(x, y - 1)
				break
			}
		case 1:
			if environmentMap[x + 1][y - 1].isWalkable && creature.distanceTo(sX - 1, sY + 1) <= 10 {
				creature.move(x + 1, y - 1)
				break
			}
		case 2:
			if environmentMap[x + 1][y].isWalkable && creature.distanceTo(sX - 1, sY) <= 10 {
				creature.move(x + 1, y)
				break
			}
		case 3:
			if environmentMap[x + 1][y + 1].isWalkable && creature.distanceTo(sX - 1, sY - 1) <= 10 {
				creature.move(x + 1, y + 1)
				break
			}
		case 4:
			if environmentMap[x][y + 1].isWalkable && creature.distanceTo(sX, sY - 1) <= 10 {
				creature.move(x, y + 1)
				break
			}
		case 5:
			if environmentMap[x - 1][y - 1].isWalkable && creature.distanceTo(sX + 1, sY + 1) <= 10 {
				creature.move(x - 1, y - 1)
				break
			}
		case 6:
			if environmentMap[x - 1][y].isWalkable && creature.distanceTo(sX + 1, sY) <= 10 {
				creature.move(x- 1, y)
				break
			}
		case 7:
			if environmentMap[x - 1][y + 1].isWalkable && creature.distanceTo(sX + 1, sY - 1) <= 10 {
				creature.move(x - 1, y + 1)
				break
			}
		}

		delay := time.Millisecond * time.Duration(rand.Intn(1000))
		creature.Actor.cooldowns["MOVEMENT"] = time.Now().Add(delay)
	}
}

func (victim *Creature) attacked(attacker string, damage int) {
	victim.Health -= damage
	if victim.Health <= 0 {
		x, y := sliceMap(victim.X, victim.Y)
		delete(creatureMap[x][y], victim.Name)
		sendActorRemoved(victim.Actor, NONE)
		victim.Actor.cooldowns["DEAD"] = time.Now()
	} else {
		sendCreatureUpdate(*victim)
	}
}
