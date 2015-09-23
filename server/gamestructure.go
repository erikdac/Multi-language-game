package main

import "fmt"

var mapSection [][](map[string]*Player)

func Test() {
	mapSection := [10][10](map[string]*Player){}
	mapSection[0][1] = make(map[string]*Player)
	player := new(Player)
	player.name = "lol"
	player.x = 1
	player.y = 2
	temp := mapSection[0][1]
	temp["woot"] = player

	temp2 := mapSection[0][1]
	fmt.Println(temp2["woot"].x)
}
 