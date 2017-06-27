package dbhandler

import (
	"database/sql"
	"errors"
	"crypto/rand"
	"fmt"
	"time"
	_ "github.com/go-sql-driver/mysql" // Using go-sql-driver
	"../gamestruct/entity"
)

var database = "root:1@tcp(localhost:3306)/server"

func CheckLogin(request map[string]string) (entity.Player, error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return entity.Player{}, err
	}
	defer db.Close()

	playerName, err := queryAccount(request, db)
	if err != nil {
		return entity.Player{}, errors.New("Login fail!")
	}

	player, err := queryPlayer(db, playerName)
	if err != nil {
		return entity.Player{}, errors.New("Database fail!")
	}

	query := "UPDATE accounts SET active = true WHERE username = ?"
	_, err = db.Exec(query, player.Name)
	if err != nil {
		return entity.Player{}, errors.New("Player is already online!")
	}

	return player, nil
}

// Queries the database table 'account' with the username and password and get
// the username back if it exists in the database.
func queryAccount(request map[string]string, db *sql.DB) (string, error) {
	token := request["Value"]

	query := "SELECT username FROM accounts WHERE hash = ? AND active = false"
	var playerName string
	err := db.QueryRow(query, token).Scan(&playerName)
	return playerName, err
}

// Queries the database table 'player' with the username defined as "name".
// It will retrieve the players attributes which it will use to instanziate
// a Player-struct and return it. If the database couldn't match it
// has a database missmatch.
func queryPlayer(db *sql.DB, name string) (entity.Player, error) {
	query := "SELECT * FROM players WHERE name = ?"
	rows, err := db.Query(query, name)
	defer rows.Close()
	if err != nil {
		return entity.Player{}, err
	}

	var x, y, lvl, health, mana int

	for rows.Next() {
		err := rows.Scan(&name, &x, &y, &lvl, &health, &mana)
		if err != nil {
			return entity.Player{}, err
		}
	}
	err = rows.Err()

	player := entity.NewPlayer(name, x, y)

	return player, err
}

func LogOut(player entity.Player) (error) {
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

	query = "UPDATE accounts SET active = false, hash = '', expiration = '1000-01-01 00:00:00' WHERE username = ?"
	_, err = db.Exec(query, player.Name)
	return err
}

func Authenticate(request map[string]string) (string, error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return "", err
	}
	defer db.Close()

	username := request["Username"]
	password := request["Password"]

	query := "SELECT username FROM accounts WHERE username = ? AND password = ? AND active = false"
	var playerName string
	err = db.QueryRow(query, username, password).Scan(&playerName)
	if err == nil {
		token := generateToken()
		expirationDate := time.Now().Add(time.Minute).Format("2006-01-02 15:04:05")
		query = "UPDATE accounts SET hash = ?, expiration = ? WHERE username = ?"
		_, err = db.Exec(query, token, expirationDate, username)
		return token, err
	}
	return "", err
}

func generateToken() (string) {
	length := 64
	b := make([]byte, length)
	rand.Read(b)
	return fmt.Sprintf("%x", b)[:length]
}

// Resets the online indexes in the 'account' table to false for all players.
func ResetOnlineList() (error) {
	db, err := sql.Open("mysql", database)
	if err != nil {
		return err
	}
	defer db.Close()

	query := "UPDATE accounts SET active = false, hash = '', expiration = '1000-01-01 00:00:00'"
	_, err = db.Exec(query)

	return err
}
