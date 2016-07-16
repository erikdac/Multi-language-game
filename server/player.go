package main

import (
	"encoding/json"
	"strconv"
	"math"
	"time"
)

type Player struct {
	Name 			string
	X   			int
	Y 				int
	Level			int
	Health 			int
	max_health 		int
	Mana			int
	max_mana		int
	target			string
	last_attack		time.Time
}

func (player *Player) sendLocalMap() {
	packet := map_packet {
		Type: "Map",
		Players: player.LocalPlayerMap(),
		Environment: player.localEnvironmentMap(),
		Creatures: player.localCreatureMap(),
	}
	data,  _ := json.Marshal(packet)
	playerToClient[player.Name].sendPacket(data)
}

func (player *Player) LocalPlayerMap() ([]Player) {

	x, y := SliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range map_players[i][j] {
				if p.Name != player.Name {
					list = append(list, *p)
				}
			}
		}
	}

	return list
}

func (player *Player) localEnvironmentMap() ([]Environment) {
	x, y := SliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0)) * MAP_SLICE
	toX := int(math.Min(float64(x+2), float64(MAP_X))) * MAP_SLICE

	fromY := int(math.Max(float64(y-1), 0.0)) * MAP_SLICE
	toY := int(math.Min(float64(y+2), float64(MAP_Y))) * MAP_SLICE

	var list []Environment

	for i := fromX; i < toX; i++ {
		for j := fromY; j < toY; j++ {
			list = append(list, environment_map[i][j])
		}
	}

	return list
}

func (player *Player) localCreatureMap() ([]Creature) {

	x, y := SliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Creature

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, c := range creature_map[i][j] {
				list = append(list, *c)
			}
		}
	}

	return list
}

func (player *Player) Movement(movement map[string]string) {
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

	if math.Abs(float64(player.X - newX)) > 1 || math.Abs(float64(player.Y - newY)) > 1  || !environment_map[newX][newY].isWalkable {
		player.moveCorrection()
		return
	}

	newSectionX, newSectionY := SliceMap(newX, newY)
	oldSectionX, oldSectionY := SliceMap(player.X, player.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := map_players[oldSectionX][oldSectionY]
		delete(oldSection, player.Name)
		sendPlayerUpdate(player, true)
		player.X = newX
		player.Y = newY
		newSection := map_players[newSectionX][newSectionY]
		newSection[player.Name] = player
		player.sendLocalMap()
	} else {
		player.X = newX
		player.Y = newY
	}

	sendPlayerUpdate(player, false);
}

func (player *Player) moveCorrection() {
	packet := player_moved_packet {
		Type: "Moved",
		NewX: player.X,
		NewY: player.Y,
	}
	data,  _ := json.Marshal(packet)
	playerToClient[player.Name].sendPacket(data)
}

func (player *Player) Auto_attack() {
	if c, ok := playerToClient[player.target]; ok {
		victim := &c.player
		if time.Since(player.last_attack).Seconds() > 2 && player.distanceTo(victim) <= 1 {
			player.attack(victim, 5) // TODO: Make some calculation for the damage.
			player.last_attack = time.Now()
		}
	}
}

func (attacker *Player) attack(victim *Player, damage int) {
	victim.Health -= damage
	packet := player_attacked_packet {
		Type: "Attacked",
		Health: victim.Health,
		Attacker: attacker.Name,
	}
	data, _ := json.Marshal(packet)
	playerToClient[victim.Name].sendPacket(data)

	sendPlayerUpdate(victim, false)
}

func (player *Player) distanceTo(p *Player) (int) {
	distance := math.Hypot(float64(player.X - p.X), float64(player.Y - p.Y))
	return int(distance)
}
