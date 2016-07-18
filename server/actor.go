package main

import (
	"math"
)

type Actor struct {
	Name	string
	X   	int
	Y 		int
}

func (actor *Actor) LocalPlayerMap() ([]Player) {

	x, y := SliceMap(actor.X, actor.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range map_players[i][j] {
				if p.Name != actor.Name {
					list = append(list, *p)
				}
			}
		}
	}

	return list
}

func (actor *Actor) distanceTo(x int, y int) (int) {
	distance := math.Hypot(float64(actor.X - x), float64(actor.Y - y))
	return int(distance)
}
