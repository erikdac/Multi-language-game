package gamestruct

import (
	"os"
	"errors"
	"bufio"
	"strings"
	"strconv"
	"../dbhandler"
	"../nethandler/packets"
	"./entity"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
	ENVIRONMENT_FILE = "gamestruct/resources/map.mf"
	SPAWN_FILE = "gamestruct/resources/spawns.mf"
)

// Binds the player names to their clients.
var NameToClient = make(map[string]*Client)

var CreatureList = make(map[string]*entity.Creature)

var playerMap [MAP_X] [MAP_Y] (map[string]*entity.Player)
var creatureMap [MAP_X] [MAP_Y] (map[string]*entity.Creature)
var environmentMap [MAP_X * MAP_SLICE] [MAP_Y * MAP_SLICE] entity.Environment

func InitiateGame() (error) {
	err := dbhandler.ResetOnlineList()
	if err != nil {
		return errors.New("Error clearing the database online list!")
	}	
	
	for i := 0; i < MAP_X; i++ {
		for j := 0; j < MAP_Y; j++ {
			playerMap[i][j] = map[string]*entity.Player{}
			creatureMap[i][j] = map[string]*entity.Creature{}
		}
	}

	err = loadMap()
	if err != nil {
		return err
	}

	return nil
}

func loadMap() (error) {
	err := loadEnvironment()
	if err != nil {
		return err
	}
	err = loadSpawns()
	if err != nil {
		return err
	}

	return nil
}

func loadEnvironment() (error) {
	file, err := os.Open(ENVIRONMENT_FILE)
	if err != nil {
		return errors.New("Environment map could not be loaded!")
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
    for scanner.Scan() {
    	line := scanner.Text()
    	v := strings.Split(line, " ")
    	env := v[0]
		x, _ := strconv.Atoi(v[1])
		y, _ := strconv.Atoi(v[2])
		isWalkable := (v[3] == "T")
		e := entity.NewEnvironment(env, x, y, isWalkable)
    	environmentMap[e.X][e.Y] = e
    }
	return nil
}

func loadSpawns() (error) {
	file, err := os.Open(SPAWN_FILE)
	if err != nil {
		return errors.New("Creature map could not be loaded!")
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
    for scanner.Scan() {
    	line := scanner.Text()
    	v := strings.Split(line, " ")
    	creatureType := v[0]
    	name := v[1]
		x, _ := strconv.Atoi(v[2])
		y, _ := strconv.Atoi(v[3])
		c := entity.NewCreature(creatureType, name, x, y, 10)
    	secX, secY := sliceMap(x, y)
    	creatureMap[secX][secY][c.Name] = c
    	CreatureList[c.Name] = c
    }
	return nil
}

func sliceMap(x int, y int) (int, int) {
	var row, column int
	if x == 0 {
		column = 0
	} else {
		column = x / MAP_SLICE
	}

	if y == 0 {
		row = 0
	} else {
		row = y / MAP_SLICE
	}

	return column, row
}

func AddClient(client *Client) {
	player := &client.Player
	NameToClient[player.Name] = client
	sendLocalMap(*player)
	x, y := sliceMap(player.X, player.Y)
	section := playerMap[x][y]
	section[player.Name] = player
	sendPlayerUpdate(player);
}

func RemovePlayer(player *entity.Player) (error) {
	x, y := sliceMap(player.X, player.Y)
	section := playerMap[x][y]
	if _, ok := section[player.Name]; ok {
		delete(NameToClient, player.Name)
		delete(section, player.Name)
		sendActorRemoved(player.Actor, NONE)
		return nil
	} else {
		return errors.New("Player '" + player.Name + "' could not be found on the map. ")
	}
}

func sendPlayerUpdate(player *entity.Player) {
	data,  _ := packets.PlayerUpdate(*player)
	for _, p := range localPlayerMap(player.Actor) {
		NameToClient[p.Name].sendPacket(data)
	}
}

func sendCreatureUpdate(creature entity.Creature) {
	data,  _ := packets.CreatureUpdate(creature)
	for _, p := range localPlayerMap(creature.Actor) {
		NameToClient[p.Name].sendPacket(data)
	}
}

type Direction int
const ( 
	NONE Direction = 1 << iota
	NORTH
	EAST
	SOUTH
	WEST
)

// The direction represents the direction that the actor is moving in.
// NONE means that the actor was removed. 
func sendActorRemoved(actor entity.Actor, direction Direction) {
	x, y := sliceMap(actor.X, actor.Y)
	data, _ := packets.ActorRemoved(actor)

	switch direction {
	case NONE:
		for _, p := range localPlayerMap(actor) {
			NameToClient[p.Name].sendPacket(data)
		}
		break
	case NORTH:
		for i := x-1; i <= x+1; i++ {
			for _, p := range playerMap[i][y+1] {
				if p.Name != actor.Name {
					NameToClient[p.Name].sendPacket(data)
				}
			}
		}
		break
	case EAST:
		for i := y-1; i <= y+1; i++ {
			for _, p := range playerMap[x-1][i] {
				if p.Name != actor.Name {
					NameToClient[p.Name].sendPacket(data)
				}
			}
		}
		break
	case SOUTH:
		for i := x-1; i <= x+1; i++ {
			for _, p := range playerMap[i][y-1] {
				if p.Name != actor.Name {
					NameToClient[p.Name].sendPacket(data)
				}
			}
		}
		break
	case WEST:
		for i := y-1; i <= y+1; i++ {
			for _, p := range playerMap[x+1][i] {
				if p.Name != actor.Name {
					NameToClient[p.Name].sendPacket(data)
				}
			}
		}
		break
	}
}
