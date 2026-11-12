package structs

import (
	"encoding/json"
	"os"
)

type Queue[T any] struct {
	data []T
}

func NewQueue[T any]() *Queue[T] {
	return &Queue[T]{data: make([]T, 0)}
}

func (q *Queue[T]) Push(val T) {
	q.data = append(q.data, val)
}

func (q *Queue[T]) Pop() bool {
	if len(q.data) == 0 {
		return false
	}
	q.data = q.data[1:]
	return true
}

func (q *Queue[T]) Front() (T, bool) {
	if len(q.data) == 0 {
		var zero T
		return zero, false
	}
	return q.data[0], true
}

func (q *Queue[T]) Empty() bool {
	return len(q.data) == 0
}

func (q *Queue[T]) SaveText(file string) error {
	data, _ := json.Marshal(q.data)
	return os.WriteFile(file, data, 0644)
}

func (q *Queue[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, &q.data)
}
