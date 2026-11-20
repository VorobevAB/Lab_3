
package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"os"
	"reflect"
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
	data, err := json.Marshal(s.data)
	if err != nil {
		return err
	}
	return os.WriteFile(file, data, 0644)
}

func (s *Stack[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, &s.data)
}

func (s *Stack[T]) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	size := int32(len(s.data))
	if err := binary.Write(f, binary.LittleEndian, size); err != nil {
		return err
	}

	for _, value := range s.data {
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

func (s *Stack[T]) LoadBin(file string) error {
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

	s.data = make([]T, 0, size)

	for i := int32(0); i < size; i++ {
		var value int32
		if err := binary.Read(f, binary.LittleEndian, &value); err != nil {
			s.data = nil
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

		s.data = append(s.data, result.Interface().(T))
	}
	return nil
}
