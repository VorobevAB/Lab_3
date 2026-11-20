
package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"os"
	"reflect"
)

type dNode[T any] struct {
	Value T
	Next  *dNode[T]
	Prev  *dNode[T]
}

type List[T any] struct {
	Head *dNode[T]
	Tail *dNode[T]
}

func NewList[T any]() *List[T] {
	return &List[T]{}
}

func (l *List[T]) LPUSH(val T) {
	node := &dNode[T]{Value: val, Next: l.Head}
	if l.Head != nil {
		l.Head.Prev = node
	} else {
		l.Tail = node
	}
	l.Head = node
}

func (l *List[T]) LPUSHB(val T) {
	node := &dNode[T]{Value: val, Prev: l.Tail}
	if l.Tail != nil {
		l.Tail.Next = node
	} else {
		l.Head = node
	}
	l.Tail = node
}

func (l *List[T]) LPUSH_AFTER(index int, val T) bool {
	if index < 0 {
		return false
	}
	curr := l.Head
	for i := 0; curr != nil && i < index; i++ {
		curr = curr.Next
	}
	if curr == nil {
		return false
	}

	node := &dNode[T]{Value: val, Prev: curr, Next: curr.Next}
	if curr.Next != nil {
		curr.Next.Prev = node
	} else {
		l.Tail = node
	}
	curr.Next = node
	return true
}

func (l *List[T]) LPUSH_BEFORE(index int, val T) bool {
	if index < 0 || index > l.LLen() {
		return false
	}
	if index == 0 {
		l.LPUSH(val)
		return true
	}
	if index == l.LLen() {
		l.LPUSHB(val)
		return true
	}

	curr := l.Head
	for i := 0; i < index; i++ {
		curr = curr.Next
	}

	node := &dNode[T]{Value: val, Next: curr, Prev: curr.Prev}
	curr.Prev.Next = node
	curr.Prev = node
	return true
}

func (l *List[T]) LDEL() bool {
	if l.Head == nil {
		return false
	}
	l.Head = l.Head.Next
	if l.Head != nil {
		l.Head.Prev = nil
	} else {
		l.Tail = nil
	}
	return true
}

func (l *List[T]) LDELBACK() bool {
	if l.Tail == nil {
		return false
	}
	l.Tail = l.Tail.Prev
	if l.Tail != nil {
		l.Tail.Next = nil
	} else {
		l.Head = nil
	}
	return true
}

func (l *List[T]) LDELNODE(index int) bool {
	return l.LDEL_AT(index)
}

func (l *List[T]) LDEL_AT(index int) bool {
	if index < 0 || index >= l.LLen() {
		return false
	}

	curr := l.Head
	for i := 0; i < index; i++ {
		curr = curr.Next
	}

	if curr.Prev != nil {
		curr.Prev.Next = curr.Next
	} else {
		l.Head = curr.Next
	}

	if curr.Next != nil {
		curr.Next.Prev = curr.Prev
	} else {
		l.Tail = curr.Prev
	}
	return true
}

func (l *List[T]) LDEL_AFTER(index int) bool {
	if index < 0 {
		return false
	}
	curr := l.Head
	for i := 0; curr != nil && i < index; i++ {
		curr = curr.Next
	}
	if curr == nil || curr.Next == nil {
		return false
	}

	next := curr.Next
	curr.Next = next.Next
	if next.Next != nil {
		next.Next.Prev = curr
	} else {
		l.Tail = curr
	}
	return true
}

func (l *List[T]) LDEL_BEFORE(index int) bool {
	if index <= 0 || index >= l.LLen() {
		return false
	}
	return l.LDEL_AT(index - 1)
}

func (l *List[T]) LDELVAL(val T) bool {
	index := l.LFIND(val)
	if index == -1 {
		return false
	}
	return l.LDEL_AT(index)
}

func (l *List[T]) LFIND(val T) int {
	index := 0
	for curr := l.Head; curr != nil; curr = curr.Next {
		if reflect.DeepEqual(curr.Value, val) {
			return index
		}
		index++
	}
	return -1
}

func (l *List[T]) LLen() int {
	count := 0
	for curr := l.Head; curr != nil; curr = curr.Next {
		count++
	}
	return count
}

func (l *List[T]) LPRINT() {
	for curr := l.Head; curr != nil; curr = curr.Next {
		print(curr.Value, " ")
	}
	println()
}

func (l *List[T]) LPRINTR() {
	for curr := l.Tail; curr != nil; curr = curr.Prev {
		print(curr.Value, " ")
	}
	println()
}

func (l *List[T]) LPRINTREC(node *dNode[T]) {
	if node == nil {
		return
	}
	print(node.Value, " ")
	l.LPRINTREC(node.Next)
}

func (l *List[T]) Clear() {
	l.Head = nil
	l.Tail = nil
}

func (l *List[T]) PushFront(val T) {
	l.LPUSH(val)
}

func (l *List[T]) PushBack(val T) {
	l.LPUSHB(val)
}

func (l *List[T]) DelFront() bool {
	return l.LDEL()
}

func (l *List[T]) DelBack() bool {
	return l.LDELBACK()
}

func (l *List[T]) SaveText(file string) error {
	var values []T
	for curr := l.Head; curr != nil; curr = curr.Next {
		values = append(values, curr.Value)
	}
	data, err := json.Marshal(values)
	if err != nil {
		return err
	}
	return os.WriteFile(file, data, 0644)
}

func (l *List[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	var values []T
	if err := json.Unmarshal(data, &values); err != nil {
		return err
	}

	l.Clear()
	for _, value := range values {
		l.LPUSHB(value)
	}
	return nil
}

func (l *List[T]) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	size := int32(l.LLen())
	if err := binary.Write(f, binary.LittleEndian, size); err != nil {
		return err
	}

	for curr := l.Head; curr != nil; curr = curr.Next {
		switch value := any(curr.Value).(type) {
		case int:
			if err := binary.Write(f, binary.LittleEndian, int32(value)); err != nil {
				return err
			}
		case int32:
			if err := binary.Write(f, binary.LittleEndian, value); err != nil {
				return err
			}
		default:
			return errors.New("unsupported type for binary serialization")
		}
	}
	return nil
}

func (l *List[T]) LoadBin(file string) error {
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

	l.Clear()

	for i := int32(0); i < size; i++ {
		var value int32
		if err := binary.Read(f, binary.LittleEndian, &value); err != nil {
			l.Clear()
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

		l.LPUSHB(result.Interface().(T))
	}
	return nil
}
