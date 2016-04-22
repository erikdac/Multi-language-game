package main

import (
	"bufio"
	"os"
	"sync"
	"fmt"
	"errors"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
	MAP_FILE = "map.mf"
)

var map_mutex = &sync.Mutex{}

var map_players [MAP_X + 1][MAP_Y + 1](map[string]*Player)
// var map_environment_json [MAP_X + 1][MAP_Y + 1]()

func InitiateMapStructure() (error) {
	for i := 0; i <= MAP_X; i++ {
		for j := 0; j <= MAP_Y; j++ {
			map_players[i][j] = map[string]*Player{}
		}
	}

	err := loadMap()
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
	scanner.Split(bufio.ScanLines)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
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