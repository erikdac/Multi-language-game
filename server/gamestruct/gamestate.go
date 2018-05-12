package gamestruct

import (
	"os"
	"errors"
	"bufio"
	"strings"
	"strconv"
	"../entity"
)

type GameState struct {
	PlayerList 		map[string]*entity.Player
	CreatureList 	map[string]*entity.Creature

	playerMap 		[MAP_X][MAP_Y] (map[string]*entity.Player)
	creatureMap 	[MAP_X][MAP_Y] (map[string]*entity.Creature)
	environmentMap 	[MAP_X * MAP_SLICE][MAP_Y * MAP_SLICE]entity.Environment

	netPackets		map[string][][]byte
}

func NewGameState() (GameState) {
	gameState := GameState{}
	gameState.PlayerList = make(map[string]*entity.Player)
	gameState.CreatureList = make(map[string]*entity.Creature)
	gameState.netPackets = make(map[string][][]byte)

	for i := 0; i < MAP_X; i++ {
		for j := 0; j < MAP_Y; j++ {
			gameState.playerMap[i][j] = map[string]*entity.Player{}
			gameState.creatureMap[i][j] = map[string]*entity.Creature{}
		}
	}
	return gameState
}

func (state *GameState) loadCreatures(filepath string) (error) {
	file, err := os.Open(SPAWN_FILE)
	if err == nil {
		defer file.Close()
		scanner := bufio.NewScanner(file)
	    for scanner.Scan() {
	    	line := scanner.Text()
	    	v := strings.Split(line, " ")
	    	creatureType := v[0]
	    	name := v[1]
			x, _ := strconv.Atoi(v[2])
			y, _ := strconv.Atoi(v[3])
			creature := entity.NewCreature(creatureType, name, x, y, 10)
	    	err = state.addCreature(creature)
	    	if err != nil {
	    		break
	    	}
	    }
    }
	return err
}

func (state *GameState) loadEnvironment(filepath string) (error) {
	file, err := os.Open(ENVIRONMENT_FILE)
	if err == nil {
		defer file.Close()
		scanner := bufio.NewScanner(file)
	    for scanner.Scan() {
	    	line := scanner.Text()
	    	v := strings.Split(line, " ")
	    	env := v[0]
			x, _ := strconv.Atoi(v[1])
			y, _ := strconv.Atoi(v[2])
			isWalkable := (v[3] == "T")
			e := entity.NewEnvironment(env, x, y, isWalkable)
	    	state.environmentMap[e.X][e.Y] = e
	    }
	}
	return err
}

func (state *GameState) addPlayer(player *entity.Player) (error) {
	if state.isWithinMap(player.X, player.Y) {
		x, y := sliceMap(player.X, player.Y)
		state.PlayerList[player.Name] = player
		state.playerMap[x][y][player.Name] = player
		return nil
	} else {
		return errors.New("Player '" + player.Name + "' could not be added to the map. ")
	}
}

func (state *GameState) removePlayer(player *entity.Player) (error) {
	if state.isWithinMap(player.X, player.Y) {
		x, y := sliceMap(player.X, player.Y)
		delete(state.PlayerList, player.Name)
		delete(state.playerMap[x][y], player.Name)
		return nil
	} else {
		return errors.New("Player '" + player.Name + "' could not be found on the map. ")
	}
}

func (state *GameState) addCreature(creature *entity.Creature) (error) {
	if state.isWithinMap(creature.X, creature.Y) {
		x, y := sliceMap(creature.X, creature.Y)
		state.CreatureList[creature.Name] = creature
		state.creatureMap[x][y][creature.Name] = creature
		return nil
	} else {
		return errors.New("Creature '" + creature.Name + "' could not be added to the map. ")
	}
}

func (state *GameState) addNetPacket(name string, data []byte) {
	state.netPackets[name] = append(state.netPackets[name], data)
}

func (state *GameState) GetAndClearNetPackets() (map[string][][]byte) {
	data := state.netPackets
	state.netPackets = make(map[string][][]byte)
	return data
}

func (state *GameState) isWithinMap(x int, y int) (bool) {
	secX, secY := sliceMap(x, y)
	return 0 <= secX && secX < MAP_X && 0 <= secY && secY < MAP_Y
}
