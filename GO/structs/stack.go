package structs

import (
	"encoding/json"
	"os"
)

type Stack[T any] struct {
	data []T
}

func NewStack[T any]() *Stack[T] {
	return &Stack[T]{data: make([]T, 0)}
}

func (s *Stack[T]) Push(val T) {
	s.data = append(s.data, val)
}

func (s *Stack[T]) Pop() bool {
	if len(s.data) == 0 {
		return false
	}
	s.data = s.data[:len(s.data)-1]
	return true
}

func (s *Stack[T]) Top() (T, bool) {
	if len(s.data) == 0 {
		var zero T
		return zero, false
	}
	return s.data[len(s.data)-1], true
}

func (s *Stack[T]) Empty() bool {
	return len(s.data) == 0
}

func (s *Stack[T]) SaveText(file string) error {
	data, _ := json.Marshal(s.data)
	return os.WriteFile(file, data, 0644)
}

func (s *Stack[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, &s.data)
}
