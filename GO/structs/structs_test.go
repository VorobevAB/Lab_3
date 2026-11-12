package structs

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestArray(t *testing.T) {
	a := NewArray[int]()
	a.Push(10)
	a.Push(20)
	assert.Equal(t, 2, a.Len())

	err := a.PushIdx(1, 15)
	require.NoError(t, err)
	val, err := a.Get(1)
	require.NoError(t, err)
	assert.Equal(t, 15, val)

	err = a.Set(0, 5)
	assert.NoError(t, err)
	val, _ = a.Get(0)
	assert.Equal(t, 5, val)

	err = a.Del(0)
	assert.NoError(t, err)
	assert.Equal(t, 2, a.Len())

	assert.Error(t, a.PushIdx(10, 0))
	assert.Error(t, a.Set(5, 1))
	assert.Error(t, a.Del(10))
	_, err = a.Get(10)
	assert.Error(t, err)
}

func TestForwardList(t *testing.T) {
	fl := NewForwardList[int]()
	fl.PushFront(2)
	fl.PushFront(1)
	fl.PushBack(3)

	assert.True(t, fl.DelFront())
	assert.True(t, fl.DelFront())
	assert.True(t, fl.DelFront())

	emptyFl := NewForwardList[int]()
	assert.False(t, emptyFl.DelFront())
}

func TestList(t *testing.T) {
	l := NewList[int]()
	l.PushFront(2)
	l.PushBack(3)

	assert.True(t, l.DelFront())
	assert.True(t, l.DelBack())
	assert.False(t, l.DelFront())
	assert.False(t, l.DelBack())
}

func TestStack(t *testing.T) {
	s := NewStack[int]()
	assert.True(t, s.Empty())

	_, ok := s.Top()
	assert.False(t, ok)

	s.Push(1)
	val, ok := s.Top()
	assert.True(t, ok)
	assert.Equal(t, 1, val)

	assert.True(t, s.Pop())
	assert.False(t, s.Pop())
	assert.True(t, s.Empty())
}

func TestQueue(t *testing.T) {
	q := NewQueue[int]()
	assert.True(t, q.Empty())

	_, ok := q.Front()
	assert.False(t, ok)

	q.Push(10)
	val, ok := q.Front()
	assert.True(t, ok)
	assert.Equal(t, 10, val)

	assert.True(t, q.Pop())
	assert.False(t, q.Pop())
	assert.True(t, q.Empty())
}

func TestHashTable(t *testing.T) {
	h := NewHashTable()

	_, ok := h.Find("one")
	assert.False(t, ok)

	h.Insert("one", "1")
	h.Insert("one", "11")
	val, ok := h.Find("one")
	assert.True(t, ok)
	assert.Equal(t, "11", val)

	h.Insert("k", "K")
	assert.True(t, h.Delete("one"))
	assert.True(t, h.Delete("k"))
	assert.False(t, h.Delete("nonexistent"))
}

func TestTree(t *testing.T) {
	tr := NewTree()
	assert.False(t, tr.Find(1))

	for i := 1; i <= 7; i++ {
		tr.Insert(i)
	}

	assert.Equal(t, 7, tr.Size)
	for i := 1; i <= 7; i++ {
		assert.True(t, tr.Find(i))
	}
	assert.False(t, tr.Find(99))
}

func TestSerialization(t *testing.T) {
	dir, err := os.MkdirTemp("", "lab3_go_test")
	require.NoError(t, err)
	defer os.RemoveAll(dir)

	txtFile := dir + "/test.json"
	binFile := dir + "/test.bin"

	a := NewArray[int]()
	a.Push(1)
	a.Push(2)

	assert.NoError(t, a.SaveText(txtFile))
	assert.NoError(t, a.SaveBin(binFile))

	b := NewArray[int]()
	assert.NoError(t, b.LoadText(txtFile))
	assert.NoError(t, b.LoadBin(binFile))
	assert.Equal(t, 2, b.Len())

	assert.Error(t, b.LoadText("/nonexistent/file.json"))
	assert.Error(t, b.LoadBin("/nonexistent/file.bin"))

	fl := NewForwardList[int]()
	fl.PushBack(1)
	assert.NoError(t, fl.SaveText(txtFile))
	fl2 := NewForwardList[int]()
	assert.NoError(t, fl2.LoadText(txtFile))

	l := NewList[int]()
	l.PushBack(1)
	assert.NoError(t, l.SaveText(txtFile))
	l2 := NewList[int]()
	assert.NoError(t, l2.LoadText(txtFile))

	s := NewStack[int]()
	s.Push(1)
	assert.NoError(t, s.SaveText(txtFile))
	s2 := NewStack[int]()
	assert.NoError(t, s2.LoadText(txtFile))

	q := NewQueue[int]()
	q.Push(1)
	assert.NoError(t, q.SaveText(txtFile))
	q2 := NewQueue[int]()
	assert.NoError(t, q2.LoadText(txtFile))

	h := NewHashTable()
	h.Insert("key", "val")
	assert.NoError(t, h.SaveText(txtFile))
	h2 := NewHashTable()
	assert.NoError(t, h2.LoadText(txtFile))
	val, ok := h2.Find("key")
	assert.True(t, ok)
	assert.Equal(t, "val", val)

	tr := NewTree()
	tr.Insert(10)
	assert.NoError(t, tr.SaveText(txtFile))
	tr2 := NewTree()
	assert.NoError(t, tr2.LoadText(txtFile))
	assert.True(t, tr2.Find(10))
}
