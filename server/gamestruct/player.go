package gamestruct

import (
	"encoding/json"
	"strconv"
	"math"
	"time"
)

type Player struct {
	Actor

	Level		int
	Health 		int
	maxHealth	int
	Mana		int
	maxMana		int
	target		string
	cooldowns	map[string]time.Time
}

func (player Player) sendLocalMap() {
	packet := map_packet {
		Type: "Map",
		Players: player.localPlayerMap(),
		Environment: player.localEnvironmentMap(),
		Creatures: player.localCreatureMap(),
	}
	data,  _ := json.Marshal(packet)
	PlayerToClient[player.Name].sendPacket(data)
}

func (player Player) localEnvironmentMap() ([]Environment) {
	x, y := sliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0)) * MAP_SLICE
	toX := int(math.Min(float64(x+2), float64(MAP_X))) * MAP_SLICE

	fromY := int(math.Max(float64(y-1), 0.0)) * MAP_SLICE
	toY := int(math.Min(float64(y+2), float64(MAP_Y))) * MAP_SLICE

	var list []Environment

	for i := fromX; i < toX; i++ {
		for j := fromY; j < toY; j++ {
			list = append(list, environmentMap[i][j])
		}
	}

	return list
}

func (player Player) localCreatureMap() ([]Creature) {

	x, y := sliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Creature

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, c := range creatureMap[i][j] {
				list = append(list, *c)
			}
		}
	}

	return list
}

func (player * Player) movement(movement map[string]string) {
	newX, err := strconv.Atoi(movement["ToX"])
	if err != nil || newX < 0 || newX >= MAP_X * MAP_SLICE {
		player.moveCorrection()
		return
	}
	newY, err := strconv.Atoi(movement["ToY"])
	if err != nil || newY < 0 || newY >= MAP_Y * MAP_SLICE {
		player.moveCorrection()
		return
	}

	if math.Abs(float64(player.X - newX)) > 1 || math.Abs(float64(player.Y - newY)) > 1  || !environmentMap[newX][newY].isWalkable {
		player.moveCorrection()
		return
	}

	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(player.X, player.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := playerMap[oldSectionX][oldSectionY]
		delete(oldSection, player.Name)
		sendActorRemoved(player.Actor)
		player.X = newX
		player.Y = newY
		newSection := playerMap[newSectionX][newSectionY]
		newSection[player.Name] = player
		player.sendLocalMap()
	} else {
		player.X = newX
		player.Y = newY
	}

	sendPlayerUpdate(player);
}

func (player Player) moveCorrection() {
	packet := player_moved_packet {
		Type: "Moved",
		NewX: player.X,
		NewY: player.Y,
	}
	data,  _ := json.Marshal(packet)
	PlayerToClient[player.Name].sendPacket(data)
}

func (player * Player) Auto_attack() {
	if c, ok := PlayerToClient[player.target]; ok {
		victim := &c.Player
		if player.cooldownMS("AUTO_ATTACK") > 2000 && player.distanceTo(victim.X, victim.Y) <= 1 {
			victim.attacked(player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.cooldowns["AUTO_ATTACK"] = time.Now()
		}
	} else if victim, ok := CreatureList[player.target]; ok {
		if player.cooldownMS("AUTO_ATTACK") > 2000 && player.distanceTo(victim.X, victim.Y) <= 1 {
			victim.attacked(player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.cooldowns["AUTO_ATTACK"] = time.Now()
		}
	}
}

func (victim * Player) attacked(attacker string, damage int) {
	victim.Health -= damage
	packet := player_attacked_packet {
		Type: "Attacked",
		Health: victim.Health,
		Attacker: attacker,
	}
	data, _ := json.Marshal(packet)
	PlayerToClient[victim.Name].sendPacket(data)

	sendPlayerUpdate(victim)
}
