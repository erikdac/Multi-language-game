package main

type Creature struct {
	Actor

	Type 	string
	spawnX	int
	spawnY	int
	Health	int
}

func (creature *Creature) Process() {

}
