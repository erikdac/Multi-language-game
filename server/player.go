package main

import (
	"encoding/json"
	"strconv"
	"math"
	"sync"
	"time"
)

type Player struct {
	Name 		string
	X   		int
	Y 			int
	Level		int
	Health 		int
	max_health 	int
	Mana		int
	max_mana	int
	target		(chan map[string]string)
	mutex 		sync.Mutex
}

func (player *Player) sendLocalMap() {
	packet := map_packet {
		Type: "Map",
		Players: player.LocalPlayerMap(),
		Environment: player.localEnvironmentMap(),
	}
	data,  _ := json.Marshal(packet)
	playerToClient[player.Name].sendPacket(data)
}

func (player *Player) LocalPlayerMap() ([]Player) {

	x, y := SliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Player

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			for _, p := range map_players[i][j] {
				if p.Name != player.Name {
					p.mutex.Lock()
					list = append(list, *p)
					p.mutex.Unlock()
				}
			}
		}
	}

	return list
}

func (player *Player) localEnvironmentMap() ([]Environment) {
	x, y := SliceMap(player.X, player.Y)

	fromX := int(math.Max(float64(x-1), 0.0))
	toX := int(math.Min(float64(x+1), float64(MAP_X-1)))

	fromY := int(math.Max(float64(y-1), 0.0))
	toY := int(math.Min(float64(y+1), float64(MAP_Y-1)))

	var list []Environment

	for i := fromX; i <= toX; i++ {
		for j := fromY; j <= toY; j++ {
			list = append(list, map_environment[i][j]...)
		}
	}

	return list
}

func (player *Player) Movement(movement map[string]string) {
	newX, _ := strconv.Atoi(movement["ToX"])
	newY, _ := strconv.Atoi(movement["ToY"])

	if (math.Abs(float64(player.X - newX)) > 1 || math.Abs(float64(player.Y - newY)) > 1) {
		packet := player_moved_packet {
			Type: "Moved",
			NewX: player.X,
			NewY: player.Y,
		}
		data,  _ := json.Marshal(packet)
		playerToClient[player.Name].sendPacket(data)
		return;
	}

	newSectionX, newSectionY := SliceMap(newX, newY)
	oldSectionX, oldSectionY := SliceMap(player.X, player.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := map_players[oldSectionX][oldSectionY]
		newSection := map_players[newSectionX][newSectionY]
		delete(oldSection, player.Name)
		sendPlayerUpdate(player, true)
		player.X = newX
		player.Y = newY
		newSection[player.Name] = player
		player.sendLocalMap()
	} else {
		player.X = newX
		player.Y = newY
	}

	sendPlayerUpdate(player, false);
}

// TODO: Clean up this function!
func (player *Player) Auto_attack() {
	var target_mutex = &sync.Mutex{}
	var target string
    blocking := &sync.Cond{L: &sync.Mutex{}}
	blocking.L.Lock()
	running := true
	paused := false
	go func() {
		for running == true {
			data := <- player.target
			if data["Condition"] == "Start" {
				target_mutex.Lock()
				target = data["Victim"]
				target_mutex.Unlock()
				paused = false
				blocking.Signal()
			} else if data["Condition"] == "Stop" {
				paused = true
			} else if data["Condition"] == "Shutdown" {
				running = false
				paused = false
				blocking.Signal()
			}
		}
	}()

	blocking.Wait()
	for running == true {
	    target_mutex.Lock()
	    victim := playerList[target];
	    target_mutex.Unlock()
	    if victim == nil {
			running = false
			paused = false
		} else if player.distanceToPlayer(victim) <= 1 {
	    	go playerAttacked(victim, player.Name, 5)	// TODO: Make some calculation for the damage.
	    	time.Sleep(2000 * time.Millisecond)
	    } else {
	    	time.Sleep(100 * time.Millisecond)
	    }
	    if paused {
			blocking.Wait()
	    }
	}
}

func (player *Player) distanceToPlayer(p *Player) (int) {
	distance := math.Hypot(float64(player.X - p.X), float64(player.Y - p.Y))
	return int(distance)
}
