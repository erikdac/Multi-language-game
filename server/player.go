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

	// Parameters not to clients to server should NOT be capitalized.
}

func (player *Player) sendLocalMap() {
	packet := map_packet {
		Type: "Map",
		Players: player.LocalPlayerMap(),
	}
	data,  _ := json.Marshal(packet)
	clientList[player.Name].write(data)
}

func (player *Player) LocalPlayerMap() ([]Player) {

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
					p.mutex.Lock()
					list = append(list, *p)
					p.mutex.Unlock()
				}
			}
		}
	}
	map_mutex.Unlock()

	return list
}

func (player *Player) Movement(movement map[string]string) {
	newX, _ := strconv.Atoi(movement["ToX"])
	newY, _ := strconv.Atoi(movement["ToY"])
	newSectionX, newSectionY := sliceMap(newX, newY)
	oldSectionX, oldSectionY := sliceMap(player.X, player.Y)
	if newSectionX != oldSectionX || newSectionY != oldSectionY {
		oldSection := mapSection[oldSectionX][oldSectionY]
		newSection := mapSection[newSectionX][newSectionY]
		map_mutex.Lock()
		delete(oldSection, player.Name)
		map_mutex.Unlock()
		sendPlayerUpdate(player, true)
		player.mutex.Lock()
		player.X = newX
		player.Y = newY
		player.mutex.Unlock()
		map_mutex.Lock()
		newSection[player.Name] = player
		map_mutex.Unlock()
		player.sendLocalMap()
	} else {
		player.mutex.Lock()
		player.X = newX
		player.Y = newY
		player.mutex.Unlock()
	}

	sendPlayerUpdate(player, false);
}

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
	    victim := playerList[target]
	    target_mutex.Unlock()
	    if player.distanceToPlayer(victim) <= 1 {
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
	player.mutex.Lock()
	p.mutex.Lock()
	distance := math.Hypot(float64(player.X - p.X), float64(player.Y - p.Y))
	p.mutex.Unlock()
	player.mutex.Unlock()
	return int(distance)
}
