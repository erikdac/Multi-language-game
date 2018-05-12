package gamestruct

import (
	"time"
	"math/rand"
	"../entity"
)

const (
	RESPAWN_TIME = 30000
)

func ProcessCreature(state *GameState, creature *entity.Creature) {
	if creature.Health <= 0 {
		deadCreature(state, creature)
	} else if len(creature.Target()) > 0 {
		combat(state, creature)
		checkTarget(state, creature)
	} else {
		outOfCombat(state, creature)
		findTarget(state, creature)
	}
}

func deadCreature(state *GameState, creature *entity.Creature) {
	if creature.Actor.CooldownMS("DEAD") > RESPAWN_TIME {
		creature.Resurrect()
		state.addCreature(creature)
		sendCreatureUpdate(state, *creature)
	}
}

func checkTarget(state *GameState, creature *entity.Creature) {
	if _, ok := state.PlayerList[creature.Target()]; !ok {
		creature.SetTarget("")
		findTarget(state, creature)
	} else {
		target := state.PlayerList[creature.Target()]
		if creature.Actor.DistanceTo(target.X, target.Y) > 5 {
			creature.SetTarget("")
			findTarget(state, creature)
		}
	}
}

func findTarget(state *GameState, creature *entity.Creature) {
	shortestDistance := 5
	for _, p := range localPlayerMap(state, creature.Actor) {
		dist := creature.Actor.DistanceTo(p.X, p.Y)
		if dist <= shortestDistance {
			shortestDistance = dist
			creature.SetTarget(p.Name)
		}
	}
}

func combat(state *GameState, creature *entity.Creature) {

}

func move(state *GameState, creature *entity.Creature, newX int, newY int) {
	newSecX, newSecY := sliceMap(newX, newY)
	oldSecX, oldSecY := sliceMap(creature.X, creature.Y)

	if newSecX != oldSecX || newSecY != oldSecY {
		oldSection := state.creatureMap[oldSecX][oldSecY]
		delete(oldSection, creature.Name)
		if newSecY < oldSecY {
			sendActorRemoved(state, creature.Actor, NORTH)
		} else if newSecX > oldSecX {
			sendActorRemoved(state, creature.Actor, EAST)
		} else if newSecY > oldSecY {
			sendActorRemoved(state, creature.Actor, SOUTH)
		} else if newSecX < oldSecX {
			sendActorRemoved(state, creature.Actor, WEST)
		}
		creature.X = newX
		creature.Y = newY
		newSection := state.creatureMap[newSecX][newSecY]
		newSection[creature.Name] = creature
	} else {
		creature.X = newX
		creature.Y = newY
	}
	sendCreatureUpdate(state, *creature)
}

func outOfCombat(state *GameState, creature *entity.Creature) {
	regnerate(state, creature)
	walk(state, creature)
}

func regnerate(state *GameState, creature *entity.Creature) {
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
		sendCreatureUpdate(state, *creature)
	}
}

func allowedWalk(state *GameState, creature *entity.Creature, newX int, newY int) bool {
	sX, sY := creature.SpawnLocation()
	xDiff := creature.X - newX
	yDiff := creature.Y - newY

	return state.environmentMap[newX][newY].IsWalkable() &&
		creature.Actor.DistanceTo(sX + xDiff, sY + yDiff) <= 10
}

func walk(state *GameState, creature *entity.Creature) {
	if creature.Actor.CooldownMS("MOVEMENT") > 5000 {
		newX := creature.X + (rand.Intn(2) - 1)
		newY := creature.Y + (rand.Intn(2) - 1)

		if (newX != creature.X || newY != creature.Y) && allowedWalk(state, creature, newX, newY) {
			move(state, creature, newX, newY)
			delay := time.Millisecond * time.Duration(rand.Intn(1000))
			creature.Actor.AddCooldown("MOVEMENT", time.Now().Add(delay))
		}
	}
}

func attackedCreature(state *GameState, victim *entity.Creature, attacker string, damage int) {
	victim.Health -= damage
	if victim.Health <= 0 {
		x, y := sliceMap(victim.X, victim.Y)
		delete(state.creatureMap[x][y], victim.Name)
		sendActorRemoved(state, victim.Actor, NONE)
		victim.Actor.AddCooldown("DEAD", time.Now())
	} else {
		sendCreatureUpdate(state, *victim)
	}
}
