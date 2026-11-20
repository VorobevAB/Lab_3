
package structs

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"os"
	"reflect"
)

type fNode[T any] struct {
	Value T
	Next  *fNode[T]
}

type ForwardList[T any] struct {
	Head *fNode[T]
}

func NewForwardList[T any]() *ForwardList[T] {
	return &ForwardList[T]{}
}

func (l *ForwardList[T]) FPUSH(val T) {
	l.Head = &fNode[T]{Value: val, Next: l.Head}
}

func (l *ForwardList[T]) FPUSHB(val T) {
	node := &fNode[T]{Value: val}
	if l.Head == nil {
		l.Head = node
		return
	}
	curr := l.Head
	for curr.Next != nil {
		curr = curr.Next
	}
	curr.Next = node
}

func (l *ForwardList[T]) FPUSH_AFTER(index int, val T) bool {
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
	curr.Next = &fNode[T]{Value: val, Next: curr.Next}
	return true
}

func (l *ForwardList[T]) FPUSH_BEFORE(index int, val T) bool {
	if index < 0 || index > l.FLen() {
		return false
	}
	if index == 0 {
		l.FPUSH(val)
		return true
	}
	curr := l.Head
	for i := 0; curr != nil && i < index-1; i++ {
		curr = curr.Next
	}
	if curr == nil {
		return false
	}
	curr.Next = &fNode[T]{Value: val, Next: curr.Next}
	return true
}

func (l *ForwardList[T]) FDEL() bool {
	if l.Head == nil {
		return false
	}
	l.Head = l.Head.Next
	return true
}

func (l *ForwardList[T]) FDELBACK() bool {
	if l.Head == nil {
		return false
	}
	if l.Head.Next == nil {
		l.Head = nil
		return true
	}
	curr := l.Head
	for curr.Next.Next != nil {
		curr = curr.Next
	}
	curr.Next = nil
	return true
}

func (l *ForwardList[T]) FDELNODE(index int) bool {
	return l.FDEL_AT(index)
}

func (l *ForwardList[T]) FDEL_AT(index int) bool {
	if index < 0 || l.Head == nil {
		return false
	}
	if index == 0 {
		return l.FDEL()
	}
	curr := l.Head
	for i := 0; curr.Next != nil && i < index-1; i++ {
		curr = curr.Next
	}
	if curr.Next == nil {
		return false
	}
	curr.Next = curr.Next.Next
	return true
}

func (l *ForwardList[T]) FDEL_AFTER(index int) bool {
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
	curr.Next = curr.Next.Next
	return true
}

func (l *ForwardList[T]) FDEL_BEFORE(index int) bool {
	if index <= 0 || index >= l.FLen() {
		return false
	}
	return l.FDEL_AT(index - 1)
}

func (l *ForwardList[T]) FDELVAL(val T) bool {
	curr := l.Head
	var prev *fNode[T]

	for curr != nil {
		if reflect.DeepEqual(curr.Value, val) {
			if prev == nil {
				l.Head = curr.Next
			} else {
				prev.Next = curr.Next
			}
			return true
		}
		prev = curr
		curr = curr.Next
	}
	return false
}

func (l *ForwardList[T]) FFIND(val T) int {
	index := 0
	for curr := l.Head; curr != nil; curr = curr.Next {
		if reflect.DeepEqual(curr.Value, val) {
			return index
		}
		index++
	}
	return -1
}

func (l *ForwardList[T]) FLen() int {
	count := 0
	for curr := l.Head; curr != nil; curr = curr.Next {
		count++
	}
	return count
}

func (l *ForwardList[T]) FPRINT() {
	for curr := l.Head; curr != nil; curr = curr.Next {
		print(curr.Value, " ")
	}
	println()
}

func (l *ForwardList[T]) FPRINTREC(node *fNode[T]) {
	if node == nil {
		return
	}
	print(node.Value, " ")
	l.FPRINTREC(node.Next)
}

func (l *ForwardList[T]) Clear() {
	l.Head = nil
}

func (l *ForwardList[T]) PushFront(val T) {
	l.FPUSH(val)
}

func (l *ForwardList[T]) PushBack(val T) {
	l.FPUSHB(val)
}

func (l *ForwardList[T]) DelFront() bool {
	return l.FDEL()
}

func (l *ForwardList[T]) SaveText(file string) error {
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

func (l *ForwardList[T]) LoadText(file string) error {
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
		l.FPUSHB(value)
	}
	return nil
}

func (l *ForwardList[T]) SaveBin(file string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()

	size := int32(l.FLen())
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

func (l *ForwardList[T]) LoadBin(file string) error {
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

		l.FPUSHB(result.Interface().(T))
	}
	return nil
}
