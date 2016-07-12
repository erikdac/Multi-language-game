package main

import (
	"encoding/json"
	"errors"
)

var playerList map[string]*Player

// Binds the player names to their clients.
var playerToClient = make(map[string]*Client)

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
	section[player.Name] = player
	playerList[player.Name] = player
	go player.Auto_attack()
	sendPlayerUpdate(player, false);
}

func RemovePlayer(player *Player) {
	x, y := SliceMap(player.X, player.Y)
	section := map_players[x][y]
	player.target <- map[string]string {"Condition": "Shutdown"}
	delete(section, player.Name)
	delete(playerList, player.Name)
	sendPlayerUpdate(player, true)
}

func sendPlayerUpdate(player *Player, removed bool) {
	var temp string
	if(!removed) {
		temp = "Player_update"
	} else {
		temp = "Player_removed"
	}
	packet := player_update_packet {
		Type: temp,
		Player: *player,
	}
	data,  _ := json.Marshal(packet)

	for _, p := range player.LocalPlayerMap() {
		playerToClient[p.Name].sendPacket(data)
	}
}

func playerAttacked(victim *Player, attacker string, damage int) {
	victim.Health -= damage
	packet := player_attacked_packet {
		Type: "Attacked",
		Health: victim.Health,
		Attacker: attacker,
	}
	data, _ := json.Marshal(packet)
	playerToClient[victim.Name].sendPacket(data)

	sendPlayerUpdate(victim, false)
}
