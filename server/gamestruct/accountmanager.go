package gamestruct

import (
	"database/sql"
	"errors"
	_ "github.com/go-sql-driver/mysql" // Using go-sql-driver
//	"time"
)

var database = "root:1@tcp(localhost:3306)/server"

func checkLogin(request map[string]string) (Player, error) {

	db, err := sql.Open("mysql", database)
	if err != nil {
		return Player{}, err
	}
	defer db.Close()

	playerName, err := queryAccount(request, db)
	if err != nil {
		return Player{}, errors.New("Login fail!")
	}

	player, err := queryPlayer(db, playerName)
	if err != nil {
		return Player{}, errors.New("Database fail!")
	}

	err = setOnlineStatus(db, player.Name, true)
	if err != nil {
		return Player{}, errors.New("Player is already online!")
	}

	return player, nil
}

// Queries the database table 'account' with the username and password and get 
// the username back if it exists in the database.
func queryAccount(request map[string]string, db *sql.DB) (string, error) {
	username := request["Username"]
	password := request["Password"]

	query := "SELECT username FROM accounts WHERE username = ? AND password = ? AND active=false"
	var playerName string
	err := db.QueryRow(query, username, password).Scan(&playerName)
	return playerName, err
}

// Queries the database table 'player' with the username defined as "name". 
// It will retrieve the players attributes which it will use to instanziate 
// a Player-struct and return it. If the database couldn't match it 
// has a database missmatch. 
func queryPlayer(db *sql.DB, name string) (Player, error) {
	var player Player
	
	query := "SELECT * FROM players WHERE name = ?"
	rows, err := db.Query(query, name)
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
	player.Mana = 12
	player.target = ""
//	player.Actor.cooldowns = map[string]time.Time{}

	return player, err		
}

func logOut(player Player) (error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return err
	}
	defer db.Close()

	query := "UPDATE players SET x = ?, y = ? WHERE name = ?"
	_, err = db.Exec(query, player.X, player.Y, player.Name)
	if err != nil {
		return err
	}

	err = setOnlineStatus(db, player.Name, false)
	return err
}

func setOnlineStatus(db *sql.DB, username string, isOnline bool) (error) {
	query := "UPDATE accounts SET active = ? WHERE username = ?"
	_, err := db.Exec(query, isOnline, username)
	return err
}

// Resets the online indexes in the 'account' table to false for all players.
func resetDBOnlineList() (error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return err
	}
	defer db.Close()

	query := "UPDATE accounts SET active=false"
	_, err = db.Exec(query)

	return err
}
