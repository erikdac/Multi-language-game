package main

type login_packet struct {
	Type 		string
	Success 	bool 
	Player 		Player
}

type map_packet struct {
	Type 		string
	Players 	[]Player

	// TODO: Add creature-array
}

type player_update_packet struct {
	Type 		string
	Player 		Player
}
