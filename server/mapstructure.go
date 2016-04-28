package main

import (
	"bufio"
	"os"
	"sync"
	"errors"
	"strconv"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
	MAP_FILE = "map.mf"
	SECTION_DELIMITER = "!NEW_SECTION!"
)

type Environment struct {
	Type 	string
	x 		int
	y 		int
}

var map_mutex = &sync.Mutex{}

var map_players [MAP_X + 1][MAP_Y + 1](map[string]*Player)
var map_environment [MAP_X + 1][MAP_Y + 1]([]Environment)	// NEEDS TO HAVE MUTEX IF GONNA BE DYNAMIC!

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
	scanner.Split(bufio.ScanWords)
	for i := 0; i < MAP_X; i++ {
		for j := 0; j < MAP_Y; j++ {
			section, err := scanner.nextSection()
			if err != nil {
				return err
			}
			map_environment[i][j] = section
		}
	}
	
	return nil
}

// Used to read in each section of the map file.
func (scanner *bufio.Scanner) nextSection() ([]Environment, error) {
	size, err := strconv.Atoi(scanner.nextToken())
	if err != nil {
		return nil, err
	}
	var section []Environment

	for i := 0; i < size; i++ {
		env := scanner.nextToken()
		x, _ := strconv.Atoi(scanner.nextToken())
		y, _ := strconv.Atoi(scanner.nextToken())

		e := Environment{env, x, y}
		section = append(section, e);
	}
	return section, nil;
}

func (scanner *bufio.Scanner) nextToken() (string) {
	scanner.Scan()
	return scanner.Text()
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