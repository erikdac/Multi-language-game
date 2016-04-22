package main

import (
	"sync"
	"encoding/json"
	"fmt"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
)

var map_mutex = &sync.Mutex{}

var playerList map[string]*Player

var map_players [MAP_X + 1][MAP_Y + 1](map[string]*Player)
// var map_environment_json [MAP_X + 1][MAP_Y + 1]()

func InitiateGameStructure() (error) {

	fmt.Println("Reseting the database online list...")	
	err := resetDatabaseOnlineList()
	if err != nil {
		fmt.Println("Error clearing the database online list!")
		return err
	}	
	
	fmt.Println("Initiating the map structure...")
	initiateMapStructure()
	
	fmt.Println("Loading map file...")
	err = ReadMapFile()
	if err != nil {
		fmt.Println(err)
		return err
	}

	return nil
}

func initiateMapStructure() {
	for i := 0; i <= MAP_X; i++ {
		for j := 0; j <= MAP_Y; j++ {
			map_players[i][j] = map[string]*Player{}
		}
	}

	playerList = make(map[string]*Player)
}

func AddPlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := map_players[x][y]
	map_mutex.Lock()
	player.mutex.Lock()
	section[player.Name] = player
	playerList[player.Name] = player
	player.mutex.Unlock()
	map_mutex.Unlock()
	go player.Auto_attack()
	sendPlayerUpdate(player, false);
}

func RemovePlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := map_players[x][y]
	player.mutex.Lock()
	player.target <- map[string]string {"Condition": "Shutdown"}
	map_mutex.Lock()
	delete(section, player.Name)
	delete(playerList, player.Name)
	player.mutex.Unlock()
	map_mutex.Unlock()
	sendPlayerUpdate(player, true)
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

func sendPlayerUpdate(player *Player, removed bool) {
	var temp string
	if(!removed) {
		temp = "Player_update"
	} else {
		temp = "Player_removed"
	}
	player.mutex.Lock()
	packet := player_update_packet {
		Type: temp,
		Player: *player,
	}
	player.mutex.Unlock()
	data,  _ := json.Marshal(packet)

	for _, p := range player.LocalPlayerMap() {
		clientList[p.Name].write(data)
	}
}

func playerAttacked(victim *Player, attacker string, damage int) {
	victim.mutex.Lock()
	victim.Health -= damage
	packet := player_attacked_packet {
		Type: "Attacked",
		Health: victim.Health,
		Attacker: attacker,
	}
	data, _ := json.Marshal(packet)
	clientList[victim.Name].write(data)
	victim.mutex.Unlock()

	sendPlayerUpdate(victim, false)
}
