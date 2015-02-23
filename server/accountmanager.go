package main

import (
    "strings"
    "errors"        // lol
    "encoding/json"
)

type Login_request struct {
    Username string;
    Password string;
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
            return true;
        }
    }
    return false;
}

func checkLogin(request Login_request) (Player, error) {
    username := request.Username
    password := request.Password
    if (strings.EqualFold(username, "erik") && strings.EqualFold(password, "no")) {
        player := Player{"Erik", 10, 20}
        return player, nil
    } else {
        return Player{}, errors.New("Login_Fail") 
    }
}

/**
 * This method will read the data from the client and return it 
 * as a byte-array. It will read all the bytes until it reaches
 * an null byte. It will then return the message without the null-byte. 
 */
func (client *Client) readInput() ([]byte, error) {
    input := make([]byte, 64)
    data := []byte{}
    for {
        n, err := client.connection.Read(input)
        if err != nil {
            client.disconnect()
            break
        }

        for i := 0; i < n; i++ {
            if input[i] == 0 {
                data = append(data, input[:i]...)
                return data, nil
            }
        }
        data = append(data, input...)
    }

    return []byte{}, errors.New("Unknown_error")
}
