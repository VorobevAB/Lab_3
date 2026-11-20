
package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"os"
	"reflect"
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
	data, err := json.Marshal(q.data)
	if err != nil {
		return err
	}
	return os.WriteFile(file, data, 0644)
}

func (q *Queue[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, &q.data)
}

func (q *Queue[T]) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	size := int32(len(q.data))
	if err := binary.Write(f, binary.LittleEndian, size); err != nil {
		return err
	}

	for _, value := range q.data {
		switch v := any(value).(type) {
		case int:
			if err := binary.Write(f, binary.LittleEndian, int32(v)); err != nil {
				return err
			}
		case int32:
			if err := binary.Write(f, binary.LittleEndian, v); err != nil {
				return err
			}
		default:
			return errors.New("unsupported type for binary serialization")
		}
	}
	return nil
}

func (q *Queue[T]) LoadBin(file string) error {
	f, err := os.Open(file)
	if err != nil {
		return err
	}
	defer f.Close()

	var size int32
	if err := binary.Read(f, binary.LittleEndian, &size); err != nil {
		return err
	}
	if size < 0 {
		return errors.New("invalid file size")
	}

	var zero T
	typ := reflect.TypeOf(zero)
	if typ == nil {
		return errors.New("unsupported type")
	}

	q.data = make([]T, 0, size)

	for i := int32(0); i < size; i++ {
		var value int32
		if err := binary.Read(f, binary.LittleEndian, &value); err != nil {
			q.data = nil
			return err
		}

		result := reflect.New(typ).Elem()
		switch typ.Kind() {
		case reflect.Int:
			result.SetInt(int64(value))
		case reflect.Int32:
			result.SetInt(int64(value))
		default:
			return errors.New("unsupported type for binary serialization")
		}

		q.data = append(q.data, result.Interface().(T))
	}
	return nil
}
