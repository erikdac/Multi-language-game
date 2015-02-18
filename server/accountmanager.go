package main

import (
    "strings"
    "errors"    // lol
)

type Account struct {
    online bool
}

func (client *Client) login() bool {
    for {
        username, err := client.readInput()
        if err != nil {
            client.disconnect()
            break
        }

        password, err := client.readInput()
        if err != nil {
            client.disconnect()
            break
        }

        if checkLogin(string(username), string(password)) == true {
            client.write([]byte{1})
            return true;
        } else {
            client.write([]byte("Login failed!"))
        }
    }
    return false;
}

func checkLogin(username string, password string) bool {
    if (strings.EqualFold(username, "erik") && strings.EqualFold(password, "no")) {
        return true;
    } else {
        return false;
    }
}

/**
 * This method will read the data from the client and return it 
 * as a byte-array. It will read all the bytes until it reaches
 * an null byte. It will then read until it reaches a null-byte 
 * and returns the message without the null-byte. 
 */
func (client *Client) readInput() ([]byte, error) {
    input := make([]byte, 256)
    data := []byte{}
    for {
        n, err := client.connection.Read(input)
        if err != nil {
            client.disconnect()
            break
        }

        for i := 0; i < n; i++ {
            if input[i] == 0 && len(data) > 0 {
                return data, nil
            } else if input[i] != 0 {
                data = append(data, input[i])
            }
        }
    }
    return []byte{}, errors.New("Unknown_error")
}
