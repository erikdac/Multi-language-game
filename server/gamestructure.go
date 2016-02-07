package main

import (
	"sync"
	"encoding/json"
)

const (
	MAP_SIZE = 50
	MAP_SLICE = 14
)

var map_mutex = &sync.Mutex{}

var playerList map[string]*Player

var mapSection [MAP_SIZE + 1][MAP_SIZE + 1](map[string]*Player)

func InitiateMapStructure() {
	for i := 0; i <= MAP_SIZE; i++ {
		for j := 0; j <= MAP_SIZE; j++ {
			mapSection[i][j] = map[string]*Player{}
		}
	}

	playerList = make(map[string]*Player)
}

func AddPlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := mapSection[x][y]
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
	section := mapSection[x][y]
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
