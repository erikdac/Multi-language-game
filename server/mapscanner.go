package main

import (
	"bufio"
	"io"
	"strconv"
)

type Mapscanner struct {
	sc 	*bufio.Scanner
}

func NewMapScanner(r io.Reader) (Mapscanner) {
	scanner := Mapscanner{bufio.NewScanner(r)}
	scanner.sc.Split(bufio.ScanWords)
	return scanner
}

// Used to read in each section of the map file.
func (scanner *Mapscanner) NextSection() ([]Environment, error) {
	size, err := strconv.Atoi(scanner.nextToken())
	if err != nil {
		return nil, err
	}
	var section []Environment

	for i := 0; i < size; i++ {
		env := scanner.nextToken()
		x, _ := strconv.Atoi(scanner.nextToken())
		y, _ := strconv.Atoi(scanner.nextToken())

		e := Environment{env, x, y}
		section = append(section, e);
	}
	return section, nil;
}

func (scanner *Mapscanner) nextToken() (string) {
	scanner.sc.Scan()
	return scanner.sc.Text()
}
