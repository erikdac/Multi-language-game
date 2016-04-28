package main

type login_packet struct {
	Type 		string
	Success 	bool 
	Player 		Player
}

type map_packet struct {
	Type 		string
	Players 	[]Player
	Environment []Environment

	// TODO: Add creature-array
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
