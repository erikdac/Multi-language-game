package gamestruct

import (
	"strconv"
	"math"
	"time"
	"errors"
	"../nethandler/packets"
	"./entity"
)

const (
	AUTO_ATTACK_DELAY = 1500
)

func ProcessPlayer(player *entity.Player) {
	auto_attack(player)
}

func DeadPlayer(player *entity.Player) {
	// TODO: Something...
}

func sendLocalMap(player entity.Player) {
	players := localPlayerMap(player.Actor)
	environment := localEnvironmentMap(player)
	creatures := localCreatureMap(player)

	data, _ := packets.Map(players, environment, creatures)
	NameToClient[player.Name].sendPacket(data)
}

func localEnvironmentMap(player entity.Player) ([]entity.Environment) {
	x, y := sliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0)) * MAP_SLICE
	toX := int(math.Min(float64(x+2), float64(MAP_X))) * MAP_SLICE

	fromY := int(math.Max(float64(y-1), 0.0)) * MAP_SLICE
	toY := int(math.Min(float64(y+2), float64(MAP_Y))) * MAP_SLICE

	var list []entity.Environment

	for i := fromX; i < toX; i++ {
		for j := fromY; j < toY; j++ {
			list = append(list, environmentMap[i][j])
		}
	}

	return list
}

func localCreatureMap(player entity.Player) ([]entity.Creature) {
	x, y := sliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []entity.Creature

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, c := range creatureMap[i][j] {
				list = append(list, *c)
			}
		}
	}

	return list
}

func movement(player *entity.Player, movement map[string]string) {
	newX, newY, err := checkMoveBoundaries(*player, movement)
	if err != nil {
		moveCorrection(*player)
		return
	}

	newSecX, newSecY := sliceMap(newX, newY)
	oldSecX, oldSecY := sliceMap(player.X, player.Y)

	if newSecX != oldSecX || newSecY != oldSecY {
		oldSection := playerMap[oldSecX][oldSecY]
		delete(oldSection, player.Name)
		if newSecY < oldSecY {
			sendActorRemoved(player.Actor, NORTH)
		} else if newSecX > oldSecX {
			sendActorRemoved(player.Actor, EAST)			
		} else if newSecY > oldSecY {
			sendActorRemoved(player.Actor, SOUTH)
		} else if newSecX < oldSecX {
			sendActorRemoved(player.Actor, WEST)			
		}
		player.X = newX
		player.Y = newY
		newSection := playerMap[newSecX][newSecY]
		newSection[player.Name] = player
		sendLocalMap(*player)
	} else {
		player.X = newX
		player.Y = newY
	}

	sendPlayerUpdate(player);
}

func checkMoveBoundaries(player entity.Player, movement map[string]string) (int, int, error) {
	newX, err := strconv.Atoi(movement["ToX"])
	if err != nil {
		return 0, 0, err
	}
	newY, err := strconv.Atoi(movement["ToY"])
	if err != nil {
		return 0, 0, err
	}

	if newX < 0 || newX >= MAP_X * MAP_SLICE {
		return newX, newY, errors.New("Player out of map horizontal boundaries.")
	} 
	if newY < 0 || newY >= MAP_Y * MAP_SLICE {
		return newX, newY, errors.New("Player out of map vertical boundaries.")
	} 
	if math.Abs(float64(player.X - newX)) > 1 || math.Abs(float64(player.Y - newY)) > 1 {
		return newX, newY, errors.New("Player moved to far.")
	} 
	if !environmentMap[newX][newY].IsWalkable() {
		return newX, newY, errors.New("New position not allowed.")
	}
	
	return newX, newY, nil;
}

func moveCorrection(player entity.Player) {
	data,  _ := packets.PlayerMoved(player)
	NameToClient[player.Name].sendPacket(data)
}

func auto_attack(player *entity.Player) {
	if c, ok := NameToClient[player.Target()]; ok {
		victim := &c.Player
		if player.CooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.DistanceTo(victim.X, victim.Y) <= 1 {
			attackedPlayer(victim, player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.AddCooldown("AUTO_ATTACK", time.Now())
		}
	} else if victim, ok := CreatureList[player.Target()]; ok {
		if player.CooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.DistanceTo(victim.X, victim.Y) <= 1 {
			attackedCreature(victim, player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.AddCooldown("AUTO_ATTACK", time.Now())
		}
	}
}

func attackedPlayer(victim *entity.Player, attacker string, damage int) {
	victim.Health -= damage
	data, _ := packets.PlayerAttacked(*victim, attacker)
	NameToClient[victim.Name].sendPacket(data)
	sendPlayerUpdate(victim)
}
