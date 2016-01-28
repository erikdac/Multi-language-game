package main

import (
	"strconv"
	"sync"
	"encoding/json"
)

const (
	MAP_SIZE = 50
	MAP_SLICE = 14
)

var map_mutex = &sync.Mutex{}

var mapSection [MAP_SIZE + 1][MAP_SIZE + 1](map[string]*Player)

func InitiateMapStructure() {
	for i := 0; i <= MAP_SIZE; i++ {
		for j := 0; j <= MAP_SIZE; j++ {
			mapSection[i][j] = map[string]*Player{}
		}
	}
}

func AddPlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := mapSection[x][y]
	map_mutex.Lock()
	section[player.Name] = player
	map_mutex.Unlock()
	sendPlayerUpdate(player, false);
}

func RemovePlayer(player *Player) {
	x, y := sliceMap(player.X, player.Y)
	section := mapSection[x][y]
	map_mutex.Lock()
	delete(section, player.Name)
	map_mutex.Unlock()
	sendPlayerUpdate(player, true)
}

func Movement(player *Player, movement map[string]string) {
	newX, _ := strconv.Atoi(movement["ToX"])
	newY, _ := strconv.Atoi(movement["ToY"])
	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(player.X, player.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := mapSection[oldSectionX][oldSectionY]
		newSection := mapSection[newSectionX][newSectionY]
		map_mutex.Lock()
		delete(oldSection, player.Name)
		map_mutex.Unlock()
		sendPlayerUpdate(player, true)
		map_mutex.Lock()
		player.X = newX
		player.Y = newY
		newSection[player.Name] = player
		map_mutex.Unlock()
		player.sendLocalMap()
	} else {
		map_mutex.Lock()
		player.X = newX
		player.Y = newY
		map_mutex.Unlock()
	}

	sendPlayerUpdate(player, false);
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
	packet := player_update_packet {
		Type: temp,
		Player: *player,
	}
	data,  _ := json.Marshal(packet)

	for _, p := range player.LocalPlayerMap() {
		clientList[p.Name].write(data)
	}
}
