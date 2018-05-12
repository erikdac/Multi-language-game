package gamestruct

import (
	"strconv"
	"math"
	"time"
	"errors"
	"../nethandler/packets"
	"../entity"
)

const (
	AUTO_ATTACK_DELAY = 1500
)

func ProcessPlayer(state *GameState, player *entity.Player) {
	auto_attack(state, player)
}

func DeadPlayer(state *GameState, player *entity.Player) {
	// TODO: Something...
}

func sendLocalMap(state *GameState, actor entity.Actor) {
	players := localPlayerMap(state, actor)
	environment := localEnvironmentMap(state, actor)
	creatures := localCreatureMap(state, actor)

	data, _ := packets.Map(players, environment, creatures)
	state.addNetPacket(actor.Name, data)
}

func Movement(state *GameState, player *entity.Player, movement map[string]string) {
	newX, newY, err := checkMoveBoundaries(state, *player, movement)
	if err != nil {
		moveCorrection(state, *player)
		return
	}

	newSecX, newSecY := sliceMap(newX, newY)
	oldSecX, oldSecY := sliceMap(player.X, player.Y)

	if newSecX != oldSecX || newSecY != oldSecY {
		oldSection := state.playerMap[oldSecX][oldSecY]
		delete(oldSection, player.Name)
		if newSecY < oldSecY {
			sendActorRemoved(state, player.Actor, NORTH)
		} else if newSecX > oldSecX {
			sendActorRemoved(state, player.Actor, EAST)			
		} else if newSecY > oldSecY {
			sendActorRemoved(state, player.Actor, SOUTH)
		} else if newSecX < oldSecX {
			sendActorRemoved(state, player.Actor, WEST)			
		}
		player.X = newX
		player.Y = newY
		newSection := state.playerMap[newSecX][newSecY]
		newSection[player.Name] = player
		sendLocalMap(state, (*player).Actor)
	} else {
		player.X = newX
		player.Y = newY
	}

	sendPlayerUpdate(state, *player);
}

func checkMoveBoundaries(state *GameState, player entity.Player, movement map[string]string) (int, int, error) {
	newX, err := strconv.Atoi(movement["ToX"])
	if err != nil {
		return 0, 0, err
	}
	newY, err := strconv.Atoi(movement["ToY"])
	if err != nil {
		return 0, 0, err
	}

	if !state.isWithinMap(newX, newY) {
		return 0, 0, errors.New("Player out of map boundaries!")
	} else if math.Abs(float64(player.X - newX)) > 1 || math.Abs(float64(player.Y - newY)) > 1 {
		return newX, newY, errors.New("Player moved to far.")
	} else if !state.environmentMap[newX][newY].IsWalkable() {
		return newX, newY, errors.New("New position not allowed.")
	}
	
	return newX, newY, nil;
}

func moveCorrection(state *GameState, player entity.Player) {
	data,  _ := packets.PlayerMoved(player)
	state.addNetPacket(player.Name, data)
}

func auto_attack(state *GameState, player *entity.Player) {
	if victim, ok := state.PlayerList[player.Target()]; ok {
		if player.CooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.DistanceTo(victim.X, victim.Y) <= 1 {
			attackedPlayer(state, victim, player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.AddCooldown("AUTO_ATTACK", time.Now())
		}
	} else if victim, ok := state.CreatureList[player.Target()]; ok {
		if player.CooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.DistanceTo(victim.X, victim.Y) <= 1 {
			attackedCreature(state, victim, player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.AddCooldown("AUTO_ATTACK", time.Now())
		}
	}
}

func attackedPlayer(state *GameState, victim *entity.Player, attacker string, damage int) {
	victim.Health -= damage
	data, _ := packets.PlayerAttacked(*victim, attacker)
	state.addNetPacket(victim.Name, data)
	sendPlayerUpdate(state, *victim)
}
