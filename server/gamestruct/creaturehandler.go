package gamestruct

import (
	"time"
	"math/rand"
	"./entity"
)

const (
	RESPAWN_TIME = 30000
)

func ProcessCreature(creature *entity.Creature) {
	if creature.Health <= 0 {
		deadCreature(creature)
	} else if len(creature.Target()) > 0 {
		combat(creature)
		checkTarget(creature)
	} else {
		outOfCombat(creature)
		findTarget(creature)
	}
}

func deadCreature(creature *entity.Creature) {
	if creature.Actor.CooldownMS("DEAD") > RESPAWN_TIME {
		creature.Resurrect()
		secX, secY := sliceMap(creature.X, creature.Y)
		creatureMap[secX][secY][creature.Name] = creature
		sendCreatureUpdate(*creature)
	}
}

func checkTarget(creature *entity.Creature) {
	if _, ok := NameToClient[creature.Target()]; !ok {
		creature.SetTarget("")
		findTarget(creature)
	} else {
		target := NameToClient[creature.Target()].Player
		if creature.Actor.DistanceTo(target.X, target.Y) > 5 {
			creature.SetTarget("")
			findTarget(creature)
		}
	}
}

func findTarget(creature *entity.Creature) {
	shortestDistance := 5
	for _, p := range localPlayerMap(creature.Actor) {
		dist := creature.Actor.DistanceTo(p.X, p.Y)
		if dist <= shortestDistance {
			shortestDistance = dist
			creature.SetTarget(p.Name)
		}
	}
}

func combat(creature *entity.Creature) {

}

func move(creature *entity.Creature, newX int, newY int) {
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

func outOfCombat(creature *entity.Creature) {
	regnerate(creature)
	walk(creature)
}

func regnerate(creature *entity.Creature) {
	if creature.Health < creature.MaxHealth()  && creature.Actor.CooldownMS("REGNERATE") > 1000 {
		hp_reg := int(float64(creature.MaxHealth()) * 0.05)
		if hp_reg < 1 {
			hp_reg = 1
		}

		if creature.MaxHealth() - creature.Health  < hp_reg {
			hp_reg = creature.MaxHealth() - creature.Health
		}
		creature.Health += hp_reg
		creature.Actor.AddCooldown("REGNERATE", time.Now())
		sendCreatureUpdate(*creature)
	}
}

func allowedWalk(creature *entity.Creature, newX int, newY int) bool {
	sX, sY := creature.SpawnLocation()
	xDiff := creature.X - newX
	yDiff := creature.Y - newY

	return environmentMap[newX][newY].IsWalkable() &&
		creature.Actor.DistanceTo(sX + xDiff, sY + yDiff) <= 10
}

func walk(creature *entity.Creature) {
	if creature.Actor.CooldownMS("MOVEMENT") > 5000 {
		newX := creature.X + (rand.Intn(2) - 1)
		newY := creature.Y + (rand.Intn(2) - 1)

		if (newX != creature.X || newY != creature.Y) && allowedWalk(creature, newX, newY) {
			move(creature, newX, newY)
			delay := time.Millisecond * time.Duration(rand.Intn(1000))
			creature.Actor.AddCooldown("MOVEMENT", time.Now().Add(delay))
		}
	}
}

func attackedCreature(victim *entity.Creature, attacker string, damage int) {
	victim.Health -= damage
	if victim.Health <= 0 {
		x, y := sliceMap(victim.X, victim.Y)
		delete(creatureMap[x][y], victim.Name)
		sendActorRemoved(victim.Actor, NONE)
		victim.Actor.AddCooldown("DEAD", time.Now())
	} else {
		sendCreatureUpdate(*victim)
	}
}
