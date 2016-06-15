package main

import (
	"encoding/json"
	"errors"
)

var playerList map[string]*Player

func InitiateGameStructure() (error) {

	err := resetDatabaseOnlineList()
	if err != nil {
		return errors.New("Error clearing the database online list!")
	}	
	
	err = InitiateMapStructure()
	if err != nil {
		return err
	}

	playerList = make(map[string]*Player)

	return nil
}

func AddPlayer(player *Player) {
	x, y := SliceMap(player.X, player.Y)
	section := map_players[x][y]
	map_mutex.Lock()
	player.mutex.Lock()
	section[player.Name] = player
	playerList[player.Name] = player
	player.mutex.Unlock()
	map_mutex.Unlock()
	go player.Auto_attack()
	sendPlayerUpdate(player, false);
}

func RemovePlayer(player *Player) {
	x, y := SliceMap(player.X, player.Y)
	section := map_players[x][y]
	player.mutex.Lock()
	player.target <- map[string]string {"Condition": "Shutdown"}
	map_mutex.Lock()
	delete(section, player.Name)
	delete(playerList, player.Name)
	player.mutex.Unlock()
	map_mutex.Unlock()
	sendPlayerUpdate(player, true)
}

func sendPlayerUpdate(player *Player, removed bool) {
	var temp string
	if(!removed) {
		temp = "Player_update"
	} else {
		temp = "Player_removed"
	}
	player.mutex.Lock()
	packet := player_update_packet {
		Type: temp,
		Player: *player,
	}
	player.mutex.Unlock()
	data,  _ := json.Marshal(packet)

	for _, p := range player.LocalPlayerMap() {
		playerToClient[p.Name].sendPacket(data)
	}
}

func playerAttacked(victim *Player, attacker string, damage int) {
	victim.mutex.Lock()
	victim.Health -= damage
	packet := player_attacked_packet {
		Type: "Attacked",
		Health: victim.Health,
		Attacker: attacker,
	}
	data, _ := json.Marshal(packet)
	playerToClient[victim.Name].sendPacket(data)
	victim.mutex.Unlock()

	sendPlayerUpdate(victim, false)
}
