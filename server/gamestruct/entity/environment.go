package entity

type Environment struct {
	Type 		string
	X 			int
	Y 			int
	isWalkable	bool
}

func NewEnvironment(envType string, x int, y int, walkable bool) (Environment) {
	var e Environment
	e.Type = envType
	e.X = x
	e.Y = y
	e.isWalkable = walkable
	return e
}

func (environment Environment) IsWalkable() (bool) {
	return environment.isWalkable
}
