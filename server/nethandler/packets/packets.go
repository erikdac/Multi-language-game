package packets

import (
	"encoding/json"
	"../../gamestruct/entity"
)

func Login(success bool, player entity.Player) ([]byte, error) {
	packet := struct {
		Type 		string
		Success 	bool 
		Player 		entity.Player
	} {
		"Login_success",
		success,
		player,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func Map(players []entity.Player, environment []entity.Environment, creatures []entity.Creature) ([]byte, error) {
	packet := struct {
		Type 		string
		Players 	[]entity.Player
		Environment []entity.Environment
		Creatures	[]entity.Creature
	} {
		"Map",
		players,
		environment,
		creatures,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func EnvironmentMap(environment []entity.Environment) ([]byte, error) {
	packet := struct {
		Type 		string
		Environment []entity.Environment
	} {
		"Environment_map",
		environment,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func PlayerUpdate(player entity.Player) ([]byte, error) {
	packet := struct {
		Type 	string
		Player 	entity.Player
	} {
		"Player_update",
		player,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func PlayerAttacked(victim entity.Player, attacker string) ([]byte, error) {
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

func PlayerMoved(player entity.Player) ([]byte, error) {
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

func CreatureUpdate(creature entity.Creature) ([]byte, error) {
	packet := struct {
		Type 		string
		Creature 	entity.Creature
	} {
		"Creature_update",
		creature,
	}

	data, err := json.Marshal(packet)
	return data, err
}

func ActorRemoved(actor entity.Actor) ([]byte, error) {
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
