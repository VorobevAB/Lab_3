package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"os"
	"reflect"
)

type Array[T any] struct {
	data []T
}

func NewArray[T any]() *Array[T] {
	return &Array[T]{data: make([]T, 0)}
}

func (a *Array[T]) Push(val T) {
	a.data = append(a.data, val)
}

func (a *Array[T]) PushIdx(idx int, val T) error {
	if idx < 0 || idx > len(a.data) {
		return errors.New("index out of bounds")
	}
	a.data = append(a.data, *new(T))
	copy(a.data[idx+1:], a.data[idx:])
	a.data[idx] = val
	return nil
}

func (a *Array[T]) Get(idx int) (T, error) {
	var zero T
	if idx < 0 || idx >= len(a.data) {
		return zero, errors.New("index out of bounds")
	}
	return a.data[idx], nil
}

func (a *Array[T]) Set(idx int, val T) error {
	if idx < 0 || idx >= len(a.data) {
		return errors.New("index out of bounds")
	}
	a.data[idx] = val
	return nil
}

func (a *Array[T]) Del(idx int) error {
	if idx < 0 || idx >= len(a.data) {
		return errors.New("index out of bounds")
	}
	a.data = append(a.data[:idx], a.data[idx+1:]...)
	return nil
}

func (a *Array[T]) Len() int {
	return len(a.data)
}

func (a *Array[T]) Clear() {
	a.data = make([]T, 0)
}

func (a *Array[T]) SaveText(file string) error {
	data, err := json.Marshal(a.data)
	if err != nil {
		return err
	}
	return os.WriteFile(file, data, 0644)
}

func (a *Array[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, &a.data)
}

func (a *Array[T]) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	size := int32(len(a.data))
	binary.Write(f, binary.LittleEndian, size)

	for _, v := range a.data {
		val, ok := any(v).(int)
		if !ok {
			return errors.New("binary serialization supports only int")
		}
		binary.Write(f, binary.LittleEndian, int32(val))
	}
	return nil
}

func (a *Array[T]) LoadBin(file string) error {
	f, err := os.Open(file)
	if err != nil {
		return err
	}
	defer f.Close()

	var size int32
	if err := binary.Read(f, binary.LittleEndian, &size); err != nil {
		return err
	}

	a.Clear()
	for i := int32(0); i < size; i++ {
		var val int32
		if err := binary.Read(f, binary.LittleEndian, &val); err != nil {
			return err
		}

		var zero T
		valTyp := reflect.TypeOf(zero)
		if valTyp.Kind() == reflect.Int {
			a.Push(any(int(val)).(T))
		} else if valTyp.Kind() == reflect.Int32 {
			a.Push(any(val).(T))
		} else {
			return errors.New("unsupported type for binary serialization")
		}
	}
	return nil
}
