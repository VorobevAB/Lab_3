package structs

import (
	"encoding/json"
	"os"
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

func (l *ForwardList[T]) PushFront(val T) {
	l.Head = &fNode[T]{Value: val, Next: l.Head}
}

func (l *ForwardList[T]) PushBack(val T) {
	newNode := &fNode[T]{Value: val}
	if l.Head == nil {
		l.Head = newNode
		return
	}
	curr := l.Head
	for curr.Next != nil {
		curr = curr.Next
	}
	curr.Next = newNode
}

func (l *ForwardList[T]) DelFront() bool {
	if l.Head == nil {
		return false
	}
	l.Head = l.Head.Next
	return true
}

func (l *ForwardList[T]) SaveText(file string) error {
	var slice []T
	for curr := l.Head; curr != nil; curr = curr.Next {
		slice = append(slice, curr.Value)
	}
	data, _ := json.Marshal(slice)
	return os.WriteFile(file, data, 0644)
}

func (l *ForwardList[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	var slice []T
	if err := json.Unmarshal(data, &slice); err != nil {
		return err
	}
	l.Head = nil
	for _, v := range slice {
		l.PushBack(v)
	}
	return nil
}

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

func (l *List[T]) PushFront(val T) {
	n := &dNode[T]{Value: val, Next: l.Head}
	if l.Head != nil {
		l.Head.Prev = n
	} else {
		l.Tail = n
	}
	l.Head = n
}

func (l *List[T]) PushBack(val T) {
	n := &dNode[T]{Value: val, Prev: l.Tail}
	if l.Tail != nil {
		l.Tail.Next = n
	} else {
		l.Head = n
	}
	l.Tail = n
}

func (l *List[T]) DelFront() bool {
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

func (l *List[T]) DelBack() bool {
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

func (l *List[T]) SaveText(file string) error {
	var slice []T
	for curr := l.Head; curr != nil; curr = curr.Next {
		slice = append(slice, curr.Value)
	}
	data, _ := json.Marshal(slice)
	return os.WriteFile(file, data, 0644)
}

func (l *List[T]) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	var slice []T
	if err := json.Unmarshal(data, &slice); err != nil {
		return err
	}
	l.Head, l.Tail = nil, nil
	for _, v := range slice {
		l.PushBack(v)
	}
	return nil
}
