package main

import (
	"fmt"
)

const (
	MAP_SIZE_X = 10
	MAP_SIZE_Y = 10
	MAP_SIZE_Z = 1
)

var mapSection [MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z](map[string]*Player)

func InitiateMapStructure() {
	for i := 0; i < MAP_SIZE_X; i++ {
		for j := 0; j < MAP_SIZE_Y; j++ {
			for k := 0; k < MAP_SIZE_Z; k++ {
				mapSection[i][j][k] = map[string]*Player{}
			}
		}
	}
}

func Test() {
	player1 := Player {name: "player1", x: 30, y: 20, z: 10}
	player2 := Player {name: "player2", x: 30, y: 20, z: 10}

	temp := mapSection[2][4][0]
	temp[player1.name] = &player1
	temp[player2.name] = &player2

	temp2 := mapSection[2][4][0]
	for _, p := range temp2 {
		fmt.Println(p)
	}
}
