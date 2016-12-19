package entity

import (
	"math"
	"time"
)

type Actor struct {
	Name		string
	X   		int
	Y 			int

	target		string
	cooldowns	map[string]time.Time
}

func NewActor(name string, x int, y int) (Actor) {
	return Actor{
		name, 
		x, y,
		"",
		map[string]time.Time{},
	}
}

func (actor *Actor) AddCooldown(name string, time time.Time) {
	actor.cooldowns[name] = time
}

func (actor Actor) DistanceTo(x int, y int) (int) {
	distance := math.Hypot(float64(actor.X - x), float64(actor.Y - y))
	return int(distance)
}

func (actor Actor) CooldownMS(name string) (int64) {
	if cd, ok := actor.cooldowns[name]; ok {
		return time.Since(cd).Nanoseconds() / int64(time.Millisecond)
	} else {
		return math.MaxInt64
	}
}

func (actor *Actor) SetTarget(name string) {
	actor.target = name
}

func (actor Actor) Target() (string) {
	return actor.target
}
