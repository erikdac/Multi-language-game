package main;

import (
	"bufio"
	"os"
	"fmt"
)

const (
	MAP_FILE = "map.mf"
)

func ReadMapFile() (error) {
	file, err := os.Open(MAP_FILE)
	if err != nil {
		return err
	}
	defer file.Close() 

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}
	return nil
}
 
