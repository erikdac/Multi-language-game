
package main

import (
	"database/sql"
	"errors"                           // lol
	_ "github.com/go-sql-driver/mysql" // Using go-sql-driver
	"sync"
)

var database = "root:1@tcp(localhost:3306)/server"

var database_mutex = &sync.Mutex{}

func checkLogin(request map[string]string) (Player, error) {

	db, err := sql.Open("mysql", database)
	if err != nil {
		return Player{}, err
	}
	defer db.Close()

	player_name, err := queryAccount(request, db)
	if err != nil {
		return Player{}, errors.New("Login_Fail")
	}

	player, err := queryPlayer(db, player_name)
	if err != nil {
		return Player{}, errors.New("Database_Fail")
	}

	err = setOnlineStatus(db, player.Name)
	if err != nil {
		return Player{}, errors.New("Already_Online")
	}

	return player, nil
}

// Queries the database table 'account' with the username and password and get 
// the username back if it exists in the database.
func queryAccount(request map[string]string, db *sql.DB) (string, error) {
	username := request["Username"]
	password := request["Password"]

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
		err := rows.Scan(&player.Name, &player.X, &player.Y)
		if err != nil {
			return player, err
		}
	}
	err = rows.Err()

	// TODO: Put into SQL-database
	player.Level = 1
	player.Health = 88
	player.max_health = 100  + (player.Level - 1) * 5
	player.Mana = 12
	player.max_mana = 20 + (player.Level - 1) * 2 
	player.target = make(chan map[string]string)

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
	_, err = db.Exec(query1, player.Name)
	_, err = db.Exec(query2, player.X, player.Y, player.Name)
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
