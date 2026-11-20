
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
	if err := binary.Write(f, binary.LittleEndian, size); err != nil {
		return err
	}

	for _, value := range a.data {
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
	if size < 0 {
		return errors.New("invalid file size")
	}

	var zero T
	typ := reflect.TypeOf(zero)
	if typ == nil {
		return errors.New("unsupported type")
	}

	a.Clear()

	for i := int32(0); i < size; i++ {
		var value int32
		if err := binary.Read(f, binary.LittleEndian, &value); err != nil {
			a.Clear()
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

		a.data = append(a.data, result.Interface().(T))
	}
	return nil
}
