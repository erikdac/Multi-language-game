package network

// Abstract class should not be initialized
type Type struct {
	Type string
}

type Error struct {
	Type
	Message string
}

type LoginSuccess struct {
	Type
	result string
}

type Movement struct {
	Type
	ToX int
	ToY int
}
