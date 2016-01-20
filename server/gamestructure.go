package main

import (
	"strconv"
	"sync"
)

const (
	MAP_SIZE = 10
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
	x, y := sliceMap(player.x, player.y)
	section := mapSection[x][y]
	map_mutex.Lock()
	section[player.name] = player
	map_mutex.Unlock()
}

func RemovePlayer(player *Player) {
	x, y := sliceMap(player.x, player.y)
	section := mapSection[x][y]
	map_mutex.Lock()
	delete(section, player.name)
	map_mutex.Unlock()
}

func Movement(player *Player, movement map[string]string) {
	newX, _ := strconv.Atoi(movement["ToX"])
	newY, _ := strconv.Atoi(movement["ToY"])
	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(player.x, player.y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY{
		map_mutex.Lock()
		RemovePlayer(player)
		player.x = newX
		player.y = newY
		AddPlayer(player)
		map_mutex.Unlock()
	} else {
		map_mutex.Lock()
		player.x = newX
		player.y = newY
		map_mutex.Unlock()
	}

	// TODO: Inform nearby players that the movement occured.
}

func sliceMap(x int, y int) (int, int) {
	var row, column int
	if x == 0 {
		column = 0
	} else {
		column = x / MAP_SIZE
	}

	if y == 0 {
		row = 0
	} else {
		row = y / MAP_SIZE
	}

	return column, row
}
