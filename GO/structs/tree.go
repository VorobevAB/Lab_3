package structs

import (
	"encoding/json"
	"os"
)

type tNode struct {
	Value int
	Left  *tNode
	Right *tNode
}

type CompleteBinaryTree struct {
	Root *tNode
	Size int
}

func NewTree() *CompleteBinaryTree {
	return &CompleteBinaryTree{}
}

func (t *CompleteBinaryTree) getNode(index int) *tNode {
	if index < 1 || t.Root == nil {
		return nil
	}
	curr := t.Root
	mask := 1
	for mask <= index/2 {
		mask *= 2
	}
	mask /= 2
	for mask > 0 && curr != nil {
		if (index & mask) != 0 {
			curr = curr.Right
		} else {
			curr = curr.Left
		}
		mask /= 2
	}
	return curr
}

func (t *CompleteBinaryTree) Insert(val int) {
	n := &tNode{Value: val}
	if t.Size == 0 {
		t.Root = n
	} else {
		parent := t.getNode((t.Size + 1) / 2)
		if (t.Size+1)%2 == 0 {
			parent.Left = n
		} else {
			parent.Right = n
		}
	}
	t.Size++
}

func (t *CompleteBinaryTree) Find(val int) bool {
	for i := 1; i <= t.Size; i++ {
		if n := t.getNode(i); n != nil && n.Value == val {
			return true
		}
	}
	return false
}

func (t *CompleteBinaryTree) SaveText(file string) error {
	var slice []int
	for i := 1; i <= t.Size; i++ {
		if n := t.getNode(i); n != nil {
			slice = append(slice, n.Value)
		}
	}
	data, _ := json.Marshal(slice)
	return os.WriteFile(file, data, 0644)
}

func (t *CompleteBinaryTree) LoadText(file string) error {
	data, err := os.ReadFile(file)
	if err != nil {
		return err
	}
	var slice []int
	if err := json.Unmarshal(data, &slice); err != nil {
		return err
	}
	t.Root, t.Size = nil, 0
	for _, v := range slice {
		t.Insert(v)
	}
	return nil
}
