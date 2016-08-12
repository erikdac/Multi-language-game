package gamestruct

import (
	"os"
	"errors"
	"encoding/json"
	"bufio"
	"strings"
	"strconv"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
	ENVIRONMENT_FILE = "gamestruct/resources/map.mf"
	SPAWN_FILE = "gamestruct/resources/spawns.mf"
)

type Environment struct {
	Type 		string
	X 			int
	Y 			int
	isWalkable	bool
}

// Binds the player names to their clients.
var PlayerToClient = make(map[string]*Client)

var CreatureList = make(map[string]*Creature)

var playerMap [MAP_X][MAP_Y] (map[string]*Player)			// TODO: Fix!
var creatureMap [MAP_X][MAP_Y] (map[string]*Creature)		// TODO: Fix!
var environmentMap [MAP_X * MAP_SLICE] [MAP_Y * MAP_SLICE] Environment

func InitiateGame() (error) {
	err := resetDBOnlineList()
	if err != nil {
		return errors.New("Error clearing the database online list!")
	}	
	
	for i := 0; i < MAP_X; i++ {
		for j := 0; j < MAP_Y; j++ {
			playerMap[i][j] = map[string]*Player{}
			creatureMap[i][j] = map[string]*Creature{}
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
		e := Environment{env, x, y, isWalkable}
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
		c := NewCreature(creatureType, name, x, y, 10)
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

	PlayerToClient[player.Name] = client
	player.sendLocalMap()
	x, y := sliceMap(player.X, player.Y)
	section := playerMap[x][y]
	section[player.Name] = player
	sendPlayerUpdate(player);
}

func RemovePlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := playerMap[x][y]
	delete(section, player.Name)
	sendActorRemoved(player.Actor)
}

func sendPlayerUpdate(player *Player) {
	packet := player_update_packet {
		Type: "Player_update",
		Player: *player,
	}
	data,  _ := json.Marshal(packet)

	for _, p := range player.localPlayerMap() {
		PlayerToClient[p.Name].sendPacket(data)
	}
}

func sendCreatureUpdate(creature *Creature) {
	packet := creature_update_packet {
		Type: "Creature_update",
		Creature: *creature,
	}
	data,  _ := json.Marshal(packet)

	for _, p := range creature.localPlayerMap() {
		PlayerToClient[p.Name].sendPacket(data)
	}
}

func sendActorRemoved(actor Actor) {
	packet := actor_removed_packet {
		Type: "Actor_removed",
		Name: actor.Name,
	}
	data,  _ := json.Marshal(packet)
	for _, p := range actor.localPlayerMap() {
		PlayerToClient[p.Name].sendPacket(data)
	}
}
