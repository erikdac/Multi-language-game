package main

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
	MAP_FILE = "map.mf"
)

type Environment struct {
	Type 		string
	X 			int
	Y 			int
	isWalkable	bool
}

var playerList = make(map[string]*Player)

// Binds the player names to their clients.
var playerToClient = make(map[string]*Client)

var map_players [MAP_X][MAP_Y] (map[string]*Player)
var environment_map [MAP_X * MAP_SLICE] [MAP_Y * MAP_SLICE] Environment

func InitiateGame() (error) {

	err := resetDatabaseOnlineList()
	if err != nil {
		return errors.New("Error clearing the database online list!")
	}	
	
	for i := 0; i < MAP_X; i++ {
		for j := 0; j < MAP_Y; j++ {
			map_players[i][j] = map[string]*Player{}
		}
	}

	err = loadMap()
	if err != nil {
		return errors.New("Map could not be loaded!")
	}

	return nil
}

func loadMap() (error) {
	file, err := os.Open(MAP_FILE)
	if err != nil {
		return err
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
    	environment_map[e.X][e.Y] = e
    }
	return nil
}

func SliceMap(x int, y int) (int, int) {
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

func AddPlayer(player *Player) {
	x, y := SliceMap(player.X, player.Y)
	section := map_players[x][y]
	section[player.Name] = player
	playerList[player.Name] = player
	sendPlayerUpdate(player, false);
}

func RemovePlayer(player *Player) {
	x, y := SliceMap(player.X, player.Y)
	section := map_players[x][y]
	delete(section, player.Name)
	delete(playerList, player.Name)
	sendPlayerUpdate(player, true)
}

func sendPlayerUpdate(player *Player, removed bool) {
	var temp string
	if(!removed) {
		temp = "Player_update"
	} else {
		temp = "Player_removed"
	}
	packet := player_update_packet {
		Type: temp,
		Player: *player,
	}
	data,  _ := json.Marshal(packet)

	for _, p := range player.LocalPlayerMap() {
		playerToClient[p.Name].sendPacket(data)
	}
}
