package gamestruct

type login_packet struct {
	Type 		string
	Success 	bool 
	Player 		Player
}

type map_packet struct {
	Type 		string
	Players 	[]Player
	Environment []Environment
	Creatures	[]Creature
}

type environment_map_packet struct {
	Type 		string
	Environment []Environment
}

type player_update_packet struct {
	Type 		string
	Player 		Player
}

type player_attacked_packet struct {
	Type 		string
	Health		int
	Attacker	string
}

type player_moved_packet struct {
	Type 		string
	NewX		int
	NewY		int
}

type creature_update_packet struct {
	Type 		string
	Creature 	Creature
}

type actor_removed_packet struct {
	Type 		string
	Name 		string
}
