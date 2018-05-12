package gamestruct

import (
	"math"
	"../nethandler/packets"
	"../entity"
)

const (
	MAP_X = 50
	MAP_Y = 50
	MAP_SLICE = 14
	ENVIRONMENT_FILE = "resources/map.mf"
	SPAWN_FILE = "resources/spawns.mf"
)

func InitiateGame() (*GameState, error) {
	state := NewGameState()
	state, err := loadMap(state)
	return &state, err
}

func loadMap(state GameState) (GameState, error) {
	err := state.loadCreatures(SPAWN_FILE)
	if err != nil {
		return state, err
	}
	err = state.loadEnvironment(ENVIRONMENT_FILE)
	if err != nil {
		return state, err
	}

	return state, nil
}

func sliceMap(x int, y int) (int, int) {
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

func AddClient(state *GameState, player *entity.Player) {
	err := state.addPlayer(player)
	if err == nil {
		sendLocalMap(state, player.Actor)
		sendPlayerUpdate(state, *player);
	}
}

func RemoveClient(state *GameState, player *entity.Player) (error) {
	err := state.removePlayer(player)
	if err == nil {
		sendActorRemoved(state, player.Actor, NONE)
	}
	return err
}

func localPlayerMap(state *GameState, actor entity.Actor) ([]entity.Player) {
	// TODO: Use slices?
	x, y := sliceMap(actor.X, actor.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []entity.Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range state.playerMap[i][j] {
				if p.Name != actor.Name {
					list = append(list, *p)
				}
			}
		}
	}

	return list
}

func localEnvironmentMap(state *GameState, actor entity.Actor) ([]entity.Environment) {
	// TODO: Use slices?
	x, y := sliceMap(actor.X, actor.Y)

	fromX := int(math.Max(float64(x-1), 0.0)) * MAP_SLICE
	toX := int(math.Min(float64(x+2), float64(MAP_X))) * MAP_SLICE

	fromY := int(math.Max(float64(y-1), 0.0)) * MAP_SLICE
	toY := int(math.Min(float64(y+2), float64(MAP_Y))) * MAP_SLICE

	var list []entity.Environment

	for i := fromX; i < toX; i++ {
		for j := fromY; j < toY; j++ {
			list = append(list, state.environmentMap[i][j])
		}
	}

	return list
}

func localCreatureMap(state *GameState, actor entity.Actor) ([]entity.Creature) {
	// TODO: Use slices?
	x, y := sliceMap(actor.X, actor.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []entity.Creature

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, c := range state.creatureMap[i][j] {
				list = append(list, *c)
			}
		}
	}

	return list
}

func sendPlayerUpdate(state *GameState, player entity.Player) {
	data,  _ := packets.PlayerUpdate(player)
	for _, p := range localPlayerMap(state, player.Actor) {
		state.addNetPacket(p.Name, data)
	}
}

func sendCreatureUpdate(state *GameState, creature entity.Creature) {
	data,  _ := packets.CreatureUpdate(creature)
	for _, p := range localPlayerMap(state, creature.Actor) {
		state.addNetPacket(p.Name, data)
	}
}

type Direction int
const ( 
	NONE Direction = 1 << iota
	NORTH
	EAST
	SOUTH
	WEST
)

// The direction represents the direction that the actor is moving in.
// NONE means that the actor was removed. 
func sendActorRemoved(state *GameState, actor entity.Actor, direction Direction) {
	x, y := sliceMap(actor.X, actor.Y)
	data, _ := packets.ActorRemoved(actor)

	switch direction {
	case NONE:
		for _, p := range localPlayerMap(state, actor) {
			state.addNetPacket(p.Name, data)
		}
		break
	case NORTH:
		for i := x-1; i <= x+1; i++ {
			for _, p := range state.playerMap[i][y+1] {
				if p.Name != actor.Name {
					state.addNetPacket(p.Name, data)
				}
			}
		}
		break
	case EAST:
		for i := y-1; i <= y+1; i++ {
			for _, p := range state.playerMap[x-1][i] {
				if p.Name != actor.Name {
					state.addNetPacket(p.Name, data)
				}
			}
		}
		break
	case SOUTH:
		for i := x-1; i <= x+1; i++ {
			for _, p := range state.playerMap[i][y-1] {
				if p.Name != actor.Name {
					state.addNetPacket(p.Name, data)
				}
			}
		}
		break
	case WEST:
		for i := y-1; i <= y+1; i++ {
			for _, p := range state.playerMap[x+1][i] {
				if p.Name != actor.Name {
					state.addNetPacket(p.Name, data)
				}
			}
		}
		break
	}
}
