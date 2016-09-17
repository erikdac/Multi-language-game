package gamestruct

import (
	"strconv"
	"math"
	"time"
	"errors"
)

const (
	AUTO_ATTACK_DELAY = 1500
)

type Player struct {
	Actor

	Level		int
	Health 		int
	Mana		int
	target		string
}

func (player *Player) Process() {
	player.auto_attack()
}

func (player *Player) Died() {
	// TODO: Something...
}

func (player Player) maxHealth() (int) {
	return 100  + (player.Level - 1) * 5
}

func (player Player) sendLocalMap() {
	players := player.localPlayerMap()
	environment := player.localEnvironmentMap()
	creatures := player.localCreatureMap()

	data, _ := MapPacket(players, environment, creatures)
	NameToClient[player.Name].sendPacket(data)
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

	fromX := int(math.Max(float64(player.X-MAP_SLICE), 0.0))
	toX := int(math.Min(float64(player.X+MAP_SLICE), float64(MAP_Y*MAP_SLICE-1)))

	fromY := int(math.Max(float64(player.Y-MAP_SLICE), 0.0))
	toY := int(math.Min(float64(player.Y+MAP_SLICE), float64(MAP_Y*MAP_SLICE-1)))

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

func (player *Player) movement(movement map[string]string) {
	newX, newY, err := player.checkMoveBoundaries(movement)
	if err != nil {
		player.moveCorrection()
		return
	}

	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(player.X, player.Y)
	
	delete(playerMap[player.X][player.Y], player.Name)
	player.X = newX
	player.Y = newY
	playerMap[newX][newY][player.Name] = player

	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		player.sendLocalMap()
	}

	sendPlayerUpdate(player);
}

func (player Player) checkMoveBoundaries(movement map[string]string) (int, int, error) {
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
	if !environmentMap[newX][newY].isWalkable {
		return newX, newY, errors.New("New position not allowed.")
	}
	
	return newX, newY, nil;
}

func (player Player) moveCorrection() {
	data,  _ := PlayerMovedPacket(player)
	NameToClient[player.Name].sendPacket(data)
}

func (player *Player) auto_attack() {
	if c, ok := NameToClient[player.target]; ok {
		victim := &c.Player
		if player.cooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.distanceTo(victim.X, victim.Y) <= 1 {
			victim.attacked(player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.cooldowns["AUTO_ATTACK"] = time.Now()
		}
	} else if victim, ok := CreatureList[player.target]; ok {
		if player.cooldownMS("AUTO_ATTACK") > AUTO_ATTACK_DELAY && player.distanceTo(victim.X, victim.Y) <= 1 {
			victim.attacked(player.Name, 5) // TODO: Make some calculation for the damage.
			player.Actor.cooldowns["AUTO_ATTACK"] = time.Now()
		}
	}
}

func (victim *Player) attacked(attacker string, damage int) {
	victim.Health -= damage
	data, _ := PlayerAttackedPacket(*victim, attacker)
	NameToClient[victim.Name].sendPacket(data)
	sendPlayerUpdate(victim)
}
