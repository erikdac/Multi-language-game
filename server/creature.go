package main

import (
	"math"
)

type Creature struct {
	Type 	string
	Name	string
	X		int
	Y		int
	spawnX	int
	spawnY	int
	Health	int
}

// TODO: Remove duplicate!
func (creature *Creature) LocalPlayerMap() ([]Player) {

	x, y := SliceMap(creature.X, creature.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range map_players[i][j] {
				list = append(list, *p)
			}
		}
	}

	return list
}
