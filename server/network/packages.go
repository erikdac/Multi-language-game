package network

// Abstract class should not be initialized
type Type struct {
	Type string
}

type Error struct {
	Type
	Message string
}

type LoginRequest struct {
	Username string
	Password string
}

type LoginSuccess struct {
	Type
	Success string
}

type Movement struct {
	Type
	ToX int
	ToY int
	ToZ int
}
