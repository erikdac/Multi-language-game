package gamestruct

import (
	"encoding/json"
)

func LoginPacket(success bool, player Player) ([]byte, error) {
	packet := struct {
		Type 		string
		Success 	bool 
		Player 		Player
	} {
		"Login_success",
		success,
		player,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func MapPacket(players []Player, environment []Environment, creatures []Creature) ([]byte, error) {
	packet := struct {
		Type 		string
		Players 	[]Player
		Environment []Environment
		Creatures	[]Creature
	} {
		"Map",
		players,
		environment,
		creatures,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func EnvironmentMapPacket(environment []Environment) ([]byte, error) {
	packet := struct {
		Type 		string
		Environment []Environment
	} {
		"Environment_map",
		environment,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func PlayerUpdatePacket(player Player) ([]byte, error) {
	packet := struct {
		Type 	string
		Player 	Player
	} {
		"Player_update",
		player,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func PlayerAttackedPacket(victim Player, attacker string) ([]byte, error) {
	packet := struct {
		Type 		string
		Health 		int
		Attacker 	string
	} {
		"Attacked",
		victim.Health,
		attacker,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func PlayerMovedPacket(player Player) ([]byte, error) {
	packet := struct {
		Type 	string
		NewX 	int
		NewY 	int
	} {
		"Moved",
		player.X,
		player.Y,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func CreatureUpdatePacket(creature Creature) ([]byte, error) {
	packet := struct {
		Type 		string
		Creature 	Creature
	} {
		"Creature_update",
		creature,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func ActorRemovedPacket(actor Actor) ([]byte, error) {
	packet := struct {
		Type string
		Name string
	} {
		"Actor_removed",
		actor.Name,
	}

	data, err := json.Marshal(packet)
	return data, err
}
