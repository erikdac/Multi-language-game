package gamestruct

import (
	"math"
	"./entity"
)

func localPlayerMap(actor entity.Actor) ([]entity.Player) {
	x, y := sliceMap(actor.X, actor.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []entity.Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range playerMap[i][j] {
				if p.Name != actor.Name {
					list = append(list, *p)
				}
			}
		}
	}

	return list
}
