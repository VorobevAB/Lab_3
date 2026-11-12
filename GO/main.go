package main

import (
	"fmt"
	"GO/structs"
)

func main() {
	fmt.Println("Демонстрация Go")

	a := structs.NewArray[int]()
	a.Push(10)
	a.Push(20)
	a.PushIdx(1, 15)
	fmt.Printf("Array len: %d\n", a.Len())
	if val, err := a.Get(1); err == nil {
		fmt.Printf("Array[1]: %d\n", val)
	}

	f := structs.NewForwardList[int]()
	f.PushFront(2)
	f.PushFront(1)
	f.PushBack(3)
	fmt.Println("ForwardList created")

	l := structs.NewList[int]()
	l.PushFront(100)
	l.PushBack(200)
	l.DelFront()
	l.DelBack()
	fmt.Println("DoublyList created")

	s := structs.NewStack[int]()
	s.Push(5)
	if top, ok := s.Top(); ok {
		fmt.Printf("Stack top: %d\n", top)
	}
	s.Pop()

	q := structs.NewQueue[int]()
	q.Push(1)
	q.Push(2)
	if front, ok := q.Front(); ok {
		fmt.Printf("Queue front: %d\n", front)
	}
	q.Pop()

	h := structs.NewHashTable()
	h.Insert("name", "value")
	if val, ok := h.Find("name"); ok {
		fmt.Printf("HashTable: name=%s\n", val)
	}

	t := structs.NewTree()
	for i := 1; i <= 7; i++ {
		t.Insert(i)
	}
	fmt.Printf("Tree size: %d\n", t.Size)

	fmt.Println("\nДемонстрация сериализации")
	_ = a.SaveText("/tmp/go_demo.json")
	_ = a.SaveBin("/tmp/go_demo.bin")

	a2 := structs.NewArray[int]()
	if err := a2.LoadText("/tmp/go_demo.json"); err == nil {
		fmt.Printf("Array loaded from text, len: %d\n", a2.Len())
	}
	if err := a2.LoadBin("/tmp/go_demo.bin"); err == nil {
		fmt.Printf("Array loaded from binary, len: %d\n", a2.Len())
	}
}
