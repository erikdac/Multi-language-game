package main

import (
	"encoding/json"
	"fmt"
)

type Player struct {
	Name 	string
	X   	int
	Y 		int

	// Parameters not to clients to server should NOT be capitalized.
}

func (player *Player) sendLocalMap() {



//	time.Sleep(1 * time.Second)			// TODO: FIX!



	x, y := sliceMap(player.X, player.Y)

	var list []Player

	start_x := x-1
	end_x := x+1
	if start_x < 0 {
		start_x = 0
	} else if end_x > MAP_SIZE {
		end_x = MAP_SIZE
	}

	start_y := y-1
	end_y := y+1
	if start_y < 0 {
		start_y = 0
	} else if end_y > MAP_SIZE {
		end_y = MAP_SIZE
	}

	map_mutex.Lock()
	for i := start_x; i <= end_x; i++ {
		for j := start_y; j <= end_y; j++ {
			for _, p := range mapSection[i][j] {
				if p.Name != player.Name {
					list = append(list, *p)
				}
			}
		}
	}
	map_mutex.Unlock()

	if len(list) > 0 {
		packet := &map_packet {
			Type: "Map",
			Players: list,
		}
		data, _ := json.Marshal(packet)
		fmt.Println(string(data))
		clientList[player.Name].write(data)
	}
}
