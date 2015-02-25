package main

import (
	"database/sql"
	"encoding/json"
	"errors"                           // lol
	_ "github.com/go-sql-driver/mysql" // Using go-sql-driver
	"sync"
)

var database = "root:1@tcp(localhost:3306)/server"

var database_mutex = &sync.Mutex{}

type Login_request struct {
	Username string
	Password string
}

func (client *Client) login() bool {

	for {
		input, err := client.readInput()
		if err != nil {
			return false
		}

		var request Login_request
		err = json.Unmarshal(input, &request)
		if err != nil {
			client.write([]byte("Incorrect packaging!"))
			return false
		}

		player, err := checkLogin(request)
		if err != nil {
			client.write([]byte("Login failed!"))
		} else {
			client.player = player
			client.write([]byte{1})
			return true
		}
	}
	return false
}

// This method reads the data from the client and iterates through it until
// it reaches a null-byte in which case it will return all the data.
//
// If all the data which had been read didnt include a null-byte it will
// append it to a byte-array read another round of data and repeat this
// procedure until it has finally found a null-byte.
func (client *Client) readInput() ([]byte, error) {
	input := make([]byte, 64)
	data := []byte{}
	for {
		n, err := client.connection.Read(input)
		if err != nil {
			return []byte{}, err
		}

		for i := 0; i < n; i++ {
			if input[i] == 0 {
				data = append(data, input[:i]...)
				return data, nil
			}
		}
		data = append(data, input...)
	}
}

func checkLogin(request Login_request) (Player, error) {

	db, err := sql.Open("mysql", database)
	if err != nil {
		return Player{}, err
	}
	defer db.Close()

	player_name, err := request.queryAccount(db)
	if err != nil {
		return Player{}, errors.New("Login_Fail")
	}

	player, err := queryPlayer(db, player_name)
	if err != nil {
		return Player{}, errors.New("Database_Fail")
	}

	err = setOnlineStatus(db, player.name)
	if err != nil {
		return Player{}, errors.New("Already_Online")
	}

	return player, nil
}

// Queries the database table 'account' with the username and password and get 
// the username back if it exists in the database.
func (request *Login_request) queryAccount(db *sql.DB) (string, error) {
	username := request.Username
	password := request.Password

	query := "SELECT username FROM account WHERE username = ? AND password = ? AND online=false"
	var player_name string
	database_mutex.Lock()
	err := db.QueryRow(query, username, password).Scan(&player_name)
	database_mutex.Unlock()
	return player_name, err
}

// Queries the database table 'player' with the username defined as "name". 
// It will retrieve the players attributes which it will use to instanziate 
// a Player-struct and return it. If the database couldn't match it 
// has a database missmatch. 
func queryPlayer(db *sql.DB, name string) (Player, error) {
	var player Player
	
	query := "SELECT * FROM player WHERE name = ?"
	database_mutex.Lock()
	rows, err := db.Query(query, name)
	database_mutex.Unlock()
	defer rows.Close()
	if err != nil {
		return player, err
	}
	for rows.Next() {
		err := rows.Scan(&player.name, &player.x, &player.y)
		if err != nil {
			return player, err
		}
	}
	err = rows.Err()
	
	return player, err		
}

// Sets the 'account' table attribute "online" to true.
func setOnlineStatus(db *sql.DB, username string) (error) {
	query := "UPDATE account SET online = true WHERE username = ?"
	database_mutex.Lock()
	_, err := db.Exec(query, username)
	database_mutex.Unlock()
	return err
}

func (player *Player) logOut() (error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return err
	}
	defer db.Close()

	query1 := "UPDATE account SET online=false WHERE username = ?"
	query2 := "UPDATE player SET xpos = ?, ypos = ? WHERE name = ?"
	database_mutex.Lock()
	_, err = db.Exec(query1, player.name)
	_, err = db.Exec(query2, player.x, player.y, player.name)
	database_mutex.Unlock()
	return err
}

// Resets the online indexes in the 'account' table to false for all players.
func resetDatabaseOnlineList() (error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return err
	}
	defer db.Close()

	query := "UPDATE account SET online=false WHERE online=true"
	database_mutex.Lock()
	_, err = db.Exec(query)
	database_mutex.Unlock()

	return err
}
