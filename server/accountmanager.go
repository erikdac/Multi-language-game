package main

import (
	"database/sql"
	"encoding/json"
	"errors"                           // lol
	_ "github.com/go-sql-driver/mysql" // Using go-sql-driver
	"math/rand"
)

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
	username := request.Username
	password := request.Password

	db, err := sql.Open("mysql", "root:1@tcp(localhost:3306)/server")
	if err != nil {
		return Player{}, err
	}

	player_name, err := queryPlayerName(db, username, password)
	if err != nil {
		return Player{}, errors.New("Login_Fail")
	}
	defer db.Close()

	player := Player{player_name, rand.Intn(1337), rand.Intn(1337)}
	return player, nil
}

// Querys the database with the username and password in order to get
// the name of the player associated with the account.
func queryPlayerName(db *sql.DB, username string, password string) (string, error) {
	query := "SELECT player_name FROM account WHERE username = ? AND password = ?"
	var player_name string
	err := db.QueryRow(query, username, password).Scan(&player_name)
	return player_name, err
}
