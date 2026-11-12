package structs

import (
	"testing"
)

func BenchmarkArrayPush(b *testing.B) {
	for i := 0; i < b.N; i++ {
		a := NewArray[int]()
		for j := 0; j < 1000; j++ {
			a.Push(j)
		}
	}
}

func BenchmarkForwardListPush(b *testing.B) {
	for i := 0; i < b.N; i++ {
		l := NewForwardList[int]()
		for j := 0; j < 1000; j++ {
			l.PushFront(j)
		}
	}
}

func BenchmarkListPushBack(b *testing.B) {
	for i := 0; i < b.N; i++ {
		l := NewList[int]()
		for j := 0; j < 1000; j++ {
			l.PushBack(j)
		}
	}
}

func BenchmarkStackPush(b *testing.B) {
	for i := 0; i < b.N; i++ {
		s := NewStack[int]()
		for j := 0; j < 1000; j++ {
			s.Push(j)
		}
	}
}

func BenchmarkQueuePush(b *testing.B) {
	for i := 0; i < b.N; i++ {
		q := NewQueue[int]()
		for j := 0; j < 1000; j++ {
			q.Push(j)
		}
	}
}

func BenchmarkHashInsert(b *testing.B) {
	for i := 0; i < b.N; i++ {
		h := NewHashTable()
		for j := 0; j < 1000; j++ {
			h.Insert("key", "value")
		}
	}
}

func BenchmarkTreeInsert(b *testing.B) {
	for i := 0; i < b.N; i++ {
		tr := NewTree()
		for j := 0; j < 1000; j++ {
			tr.Insert(j)
		}
	}
}
