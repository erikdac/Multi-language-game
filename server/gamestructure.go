package main

import (
	"fmt"
)

const (
	MAP_SIZE_X = 10
	MAP_SIZE_Y = 10
)

var mapSection [MAP_SIZE_X + 1][MAP_SIZE_Y + 1](map[string]*Player)

func InitiateMapStructure() {
	for i := 0; i <= MAP_SIZE_X; i++ {
		for j := 0; j <= MAP_SIZE_Y; j++ {
			mapSection[i][j] = map[string]*Player{}
		}
	}
}

func AddPlayer(player Player) {
	section := mapSection[player.x % MAP_SIZE_X][player.y % MAP_SIZE_Y]
	section[player.name] = &player
}

func RemovePlayer(player Player) {
	section := mapSection[player.x % MAP_SIZE_X][player.y % MAP_SIZE_Y]
	delete(section, player.name)
}

func Test() {
/*
	player1 := Player {name: "player1", x: 30, y: 20}
	player2 := Player {name: "player2", x: 30, y: 20}
	AddPlayer(player1)
	AddPlayer(player2)

	RemovePlayer(player2)

	section := mapSection[player1.x % MAP_SIZE_X][player1.y % MAP_SIZE_Y]
	for _, p := range section {
		fmt.Println(p)
	}
*/
	for i := 0; i <= MAP_SIZE_X; i++ {
		for j := 0; j <= MAP_SIZE_Y; j++ {
			section := mapSection[i][j]
			for _, p := range section {
				fmt.Println(p)
			}
		}
	}
}
